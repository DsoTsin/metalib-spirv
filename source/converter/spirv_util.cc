#include "spirv_util.h"

namespace spv {
using namespace llvm;

// @see
// https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf
static std::unordered_map<std::string, mtl2spv_data_type> type_map = {
    {"char", MTL2SPV_DATA_TYPE_CHAR},
    {"int8_t", MTL2SPV_DATA_TYPE_CHAR},

    {"unsigned char", MTL2SPV_DATA_TYPE_UCHAR},
    {"uchar", MTL2SPV_DATA_TYPE_UCHAR},
    {"uint8_t", MTL2SPV_DATA_TYPE_UCHAR},

    {"short", MTL2SPV_DATA_TYPE_SHORT},
    {"int16_t", MTL2SPV_DATA_TYPE_SHORT},

    {"unsigned short", MTL2SPV_DATA_TYPE_USHORT},
    {"ushort", MTL2SPV_DATA_TYPE_USHORT},
    {"uint16_t", MTL2SPV_DATA_TYPE_USHORT},

    {"half", MTL2SPV_DATA_TYPE_HALF},
    {"half2", MTL2SPV_DATA_TYPE_HALF2},
    {"half3", MTL2SPV_DATA_TYPE_HALF3},
    {"half4", MTL2SPV_DATA_TYPE_HALF4},
    {"float", MTL2SPV_DATA_TYPE_FLOAT},
    {"float2", MTL2SPV_DATA_TYPE_FLOAT2},
    {"float3", MTL2SPV_DATA_TYPE_FLOAT3},
    {"float4", MTL2SPV_DATA_TYPE_FLOAT4},
    {"int", MTL2SPV_DATA_TYPE_INT},
    {"int32_t", MTL2SPV_DATA_TYPE_INT},
    {"int2", MTL2SPV_DATA_TYPE_INT2},
    {"int3", MTL2SPV_DATA_TYPE_INT3},
    {"int4", MTL2SPV_DATA_TYPE_INT4},
    {"uint", MTL2SPV_DATA_TYPE_UINT},
    {"uint2", MTL2SPV_DATA_TYPE_UINT2},
    {"uint3", MTL2SPV_DATA_TYPE_UINT3},
    {"uint4", MTL2SPV_DATA_TYPE_UINT4},
    {"bool", MTL2SPV_DATA_TYPE_BOOL},
    {"bool2", MTL2SPV_DATA_TYPE_BOOL2},
    {"bool3", MTL2SPV_DATA_TYPE_BOOL3},
    {"bool4", MTL2SPV_DATA_TYPE_BOOL4},
};

const char *graphics_words[] = {
    "air.command_buffer", "air.imageblock", "air.imageblock_data",
    "air.imageblock_master", "air.alias_implicit_imageblock",
    "air.alias_implicit_imageblock_render_target", "air.sample_mask_in",
    "air.post_depth_coverage", "air.position", "air.invariant",
    // interpolation
    "air.center", "air.centroid", "air.perspective", "air.flat",
    "air.vertex_id", "air.no_perspective", "air.location",
    "air.visible_padding", "air.vertex_input", "air.vertex_output",
    "air.visible_output", "air.visible_input", "air.raster_order_group",
    "air.rounding_mode", "air.ci_padding", "air.constant",
    // tessellation
    "air.control_point_index_buffer", "air.depth", "air.sample_id",
    "air.sample_mask", "air.render_target_array_index", "air.stencil",
    "air.viewport_array_index", "air.amplification_id",
    "air.amplification_count", "air.barycentric_coord", "air.base_instance",
    "air.base_vertex", "air.fragment_input", "air.front_facing", "air.distance",
    "air.point_size", "air.discard_fragment", "air.disabled"};

const char *types[] = {"air.buffer", "air.texture", "air.sampler",
                       "air.stage_in"};
const char *access[] = {"air.shared", "air.read", "air.read_write", "air.write",
                        "air.sample"};
const char *comparison[] = {"air.any", "air.greater", "air.less"};
const char *raytracing_words[] = {
    "air.geometry_id",
    "air.geometry_intersection_function_table_offset",
    "air.instance_id",
    "air.instance_intersection_function_table_offset",
    "air.max_distance",
    "air.min_distance",
    "air.opaque_primitive",
    "air.origin",
    "air.direction",
    "air.triangle",
    "air.bounding_box",
    "air.instancing",
    "air.triangle_data",
    "air.world_space_data",
    "air.world_space_direction",
    "air.world_space_origin",
    "air.payload",
    "air.accept_intersection",
    "air.clip_distance",
    "air.continue_search",
    "air.patch_id",
    "air.patch_input",
    "air.pixel_position_in_tile",
    "air.pixels_per_tile",
    "air.point_coord",
    "air.stage_in_grid_origin",
    "air.stage_in_grid_size",
};

const char *compute_words[] = {
    "air.thread_index_in_quadgroup",      "air.thread_index_in_simdgroup",
    "air.thread_index_in_threadgroup",    "air.thread_position_in_grid",
    "air.thread_position_in_threadgroup", "air.threadgroup_position_in_grid",
    "air.threadgroups_per_grid",          "air.threads_per_simdgroup",
    "air.threads_per_threadgroup",        "air.simdgroup_index_in_threadgroup",
    "air.simdgroups_per_threadgroup"};

mtl2spv_data_type convert_type(std::string const &type_name) {
  auto iter = type_map.find(type_name);
  if (iter == type_map.end()) {
    return MTL2SPV_DATA_TYPE_NONE;
  }
  return iter->second;
}

int64_t cast_operand_to_int64(MDNode *N, unsigned I) {
  return mdconst::dyn_extract<ConstantInt>(N->getOperand(I))->getZExtValue();
}

llvm::Metadata *cast_operand_or_null(llvm::MDNode *N, unsigned I) {
  if (!N)
    return nullptr;
  return N->getOperand(I);
}

std::string cast_operand_to_string(MDNode *N, unsigned I) {
  if (auto *Str = dyn_cast_or_null<MDString>(cast_operand_or_null(N, I)))
    return Str->getString().str();
  return "";
}

MDNode *cast_operand_to_mdnode(MDNode *N, unsigned I) {
  return dyn_cast_or_null<MDNode>(cast_operand_or_null(N, I));
}

Type *cast_operand_to_type(MDNode *N, unsigned I) {
  return cast<ValueAsMetadata>(N->getOperand(I))->getType();
}

std::set<std::string> getNamedMDAsStringSet(Module *M,
                                            const std::string &MDName) {
  NamedMDNode *NamedMD = M->getNamedMetadata(MDName);
  std::set<std::string> StrSet;
  if (!NamedMD)
    return StrSet;

  assert(NamedMD->getNumOperands() > 0 && "Invalid SPIR");

  for (unsigned I = 0, E = NamedMD->getNumOperands(); I != E; ++I) {
    MDNode *MD = NamedMD->getOperand(I);
    if (!MD || MD->getNumOperands() == 0)
      continue;
    for (unsigned J = 0, N = MD->getNumOperands(); J != N; ++J)
      StrSet.insert(cast_operand_to_string(MD, J));
  }

  return StrSet;
}
} // namespace spv