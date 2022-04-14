#include "spirv_builder.h"

namespace spv {
using namespace llvm;

enum class VersionNumber : uint32_t {
  // See section 2.3 of SPIR-V spec: Physical Layout of a SPIR_V Module and
  // Instruction
  SPIRV_1_0 = 0x00010000,
  SPIRV_1_1 = 0x00010100,
  SPIRV_1_2 = 0x00010200,
  SPIRV_1_3 = 0x00010300,
  SPIRV_1_4 = 0x00010400,
  // TODO: populate this enum with the latest versions (up to 1.5) once
  // translator get support of corresponding features
  MinimumVersion = SPIRV_1_0,
  MaximumVersion = SPIRV_1_4
};

Builder::Builder(const mtl2spv_options &in_options,
                 const metal::String &in_entry_name,
                 const mtl2spv_shader_type &in_shader_type, Blob &in_blob)
    : blob(in_blob), options(in_options), entry_name(in_entry_name),
      shader_type(in_shader_type) {
  /*
  header:
  | MAGIC | SPV_VERSION_NUM | GENERATOR_MAGIC | ID Bound | Reserved | Frist word
  Of Instruction stream |
  */
  blob.push_back(MagicNumber);
  blob.push_back((uint32_t)VersionNumber::SPIRV_1_0); // version number
  blob.push_back(0);                                  // generator magic
  blob.push_back(0);                                  // id bound
  blob.push_back(0);                                  // reserved
  blob.push_back(spv::MagicNumber); // first word of instruction stream

  /* Instruction */
  /* | OpCode (16bits:wrod count, 16bits:opcode enumerant) | */
  /* | Optional instruction type <id> | */
  /* | Optional instruction Result <id> | */
  /* | Operand 1 <id> | */
}
Builder::~Builder() {}

std::unordered_map<std::string, int> map__ = {
    {"struct._texture_2d_t", 0},
    {"struct._depth_2d_t", 0},
    {"struct._sampler_t", 0},
    {"air.convert.f.v2f32.u.v2i32", 0},
    {"air.sample_depth_2d.f32", 0},
    {"air.fast_fmax.f32", 0},
    {"air.texture", 0},
    {"air.buffer", 0},
    {"air.location_index", 0},
    {"air.sample", 0},
    {"air.arg_type_name", 0},
    {"air.arg_name", 0},
    {"air.buffer_size", 0},
    {"air.arg_type_size", 0},
    {"air.arg_type_align_size", 0},
    {"air.thread_position_in_grid", 0},
};

} // namespace spv