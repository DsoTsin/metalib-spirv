#pragma once
#include "metalib2spirv_p.h"

namespace spv {
enum AddressSpace {
  SPIRAS_Private,
  SPIRAS_Global,
  SPIRAS_Constant,
  SPIRAS_Local,
  SPIRAS_Generic,
  SPIRAS_GlobalDevice,
  SPIRAS_GlobalHost,
  SPIRAS_Input,
  SPIRAS_Output,
  SPIRAS_Count,
};

mtl2spv_data_type convert_type(std::string const &type_name);
int64_t cast_operand_to_int64(llvm::MDNode *N, unsigned I);
llvm::Metadata *cast_operand_or_null(llvm::MDNode *N, unsigned I);
std::string cast_operand_to_string(llvm::MDNode *N, unsigned I);
llvm::MDNode *cast_operand_to_mdnode(llvm::MDNode *N, unsigned I);
llvm::Type *cast_operand_to_type(llvm::MDNode *N, unsigned I);
std::set<std::string> getNamedMDAsStringSet(llvm::Module *M,
                                            const std::string &MDName);
} // namespace spv