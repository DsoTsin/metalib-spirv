#include "metalib2spirv_p.h"
#include <sstream>

namespace metal {
Library::Library() {}
Library::~Library() {}

int Library::count() const { return (int)functions.size(); }

const char *Library::get_name(int index) const {
  return index < (int)functions.size() ? functions[index].entry_name.c_str()
                                       : nullptr;
}

mtl2spv_shader_type Library::get_type(int index) const {
  return functions[index].shader_type;
}

} // namespace metal

mtl2spv_error metallib_open(const void *bytes, size_t num_bytes,
                            metallib_t *olib) {
  static llvm::LLVMContext context;
  std::string error;
  llvm::raw_string_ostream rso(error);
  auto lib =
      metal::load_library((const uint8_t *)bytes, num_bytes, context, rso);
  if (lib && olib) {
    *olib = (metallib_t)lib.get();
    return MTL2SPV_SUCCESS;
  } else {
    auto err = lib.takeError();
    return MTL2SPV_SUCCESS;
  }
}

void metallib_free(metallib_t lib) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    delete mtlib;
  }
}

int metallib_function_count(metallib_t lib) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    return mtlib->count();
  }
  return 0;
}

const char *metallib_function_name(metallib_t lib, int index) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    return mtlib->get_name(index);
  }
  return nullptr;
}

const mtl2spv_function *metallib_function(metallib_t lib, int index) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    return (const mtl2spv_function *)&mtlib->functions[index];
  }
  return nullptr;
}

mtl2spv_shader_type metallib_function_type(metallib_t lib, int index) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    return mtlib->get_type(index);
  }
  return MTL2SPV_SHADER_UNKNOWN;
}

void metallib2spv(metallib_t lib, const mtl2spv_options *options, int index) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    std::vector<uint32_t> spirv;
    mtlib->write_spv(*options, index, spirv);
  }
}

void metallib2spv2(metallib_t lib, const mtl2spv_options *options,
                   const char *entry_name) {
  if (lib) {
    auto mtlib = (metal::Library *)lib;
    std::vector<uint32_t> spirv;
    mtlib->write_spv(*options, entry_name, spirv);
  }
}