#pragma once
#ifdef _MSC_VER
#pragma warning(disable : 4624 4244)
#endif
#include "metalib2spirv.h"
#include "spirv/unified1/spirv.hpp"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/IR/AssemblyAnnotationWriter.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/WithColor.h"

namespace spv {
using Blob = std::vector<Id>;
} // namespace spv
namespace metal {
class String : public mtl2spv_string {
public:
  String();
  String(const char *str, size_t length = 0);
  String(const llvm::StringRef &str);

  String(String &&str) {
    buffer = str.buffer;
    length = str.length;
    capacity = str.capacity;
    str.buffer = nullptr;
    str.length = 0;
    str.capacity = 0;
  }

  String(const String &str) : String(str.c_str(), str.length) {}
  ~String();

  const char *c_str() const { return buffer; }

  bool operator==(const String &str) const {
    return length == str.length && !memcmp(buffer, str.buffer, length);
  }

  String &operator=(const String &str) {
    assign(str.c_str(), str.length);
    return *this;
  }

private:
  void assign(const char *str, size_t len);
};
} // namespace metal

namespace std {
template <> struct hash<metal::String> {
  inline size_t operator()(const metal::String &str) const {
    std::string st(str.c_str());
    std::hash<string> h;
    return h(st);
  }
};
} // namespace std

namespace metal {
class Library {
public:
  Library();
  ~Library();

  struct Function {
    String entry_name;
    mtl2spv_shader_type shader_type;
    std::vector<mtl2spv_argument> args;

    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::MemoryBuffer> buffer;

    void parse_arguments();
    void write_spirv(const mtl2spv_options &options,
                     spv::Blob &out_spirv) const;
  };

  static_assert(sizeof(std::unique_ptr<llvm::Module>) == sizeof(size_t),
                "incompatible unique_ptr size");

  int count() const;
  const char *get_name(int index) const;
  mtl2spv_shader_type get_type(int index) const;

  void write_spv(const mtl2spv_options &options, int index,
                 spv::Blob &out_spirv) const;

  void write_spv(const mtl2spv_options &options, const String &name,
                 spv::Blob &out_spirv) const;

private:
  llvm::SmallVector<Function, 10> functions;
  std::unordered_map<String, size_t> map;

  friend llvm::Expected<Library *> load_library(const uint8_t *bytes,
                                                size_t num_bytes,
                                                llvm::LLVMContext &context,
                                                llvm::raw_ostream &os);
  friend const mtl2spv_function * ::metallib_function(metallib_t lib,
                                                      int index);
};
} // namespace metal
