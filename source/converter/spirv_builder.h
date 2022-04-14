#pragma once

#include "spirv_util.h"

namespace spv {
class Instruction {
public:
  Instruction(class Builder &in_builder)
      : builder(in_builder), num_words(0), op(OpMax), id(0) {}

  virtual ~Instruction() {}
  virtual bool validate() const { return false; }

protected:
  Builder &builder;
  uint16_t num_words;
  Op op;
  Id id; // instruction id
  std::string name;
};

class Builder {
public:
  Builder(const mtl2spv_options &options, const metal::String &entry_name,
          const mtl2spv_shader_type &shader_type, Blob &blob);
  ~Builder();

  friend class Writer;

private:
  Blob &blob;
  const mtl2spv_options &options;
  const metal::String &entry_name;
  const mtl2spv_shader_type &shader_type;
};
} // namespace spv