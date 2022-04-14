#include "metalib2spirv_p.h"
#include "spirv_builder.h"

namespace spv {
using namespace llvm;

class Writer {
public:
  Writer(const llvm::Module &mod, Builder &builder);

  void generate();
  void extract_meta();

  // Instruction / BasicBlock / Function, inherits from Value
  void translate(const Function &func);
  void translate_inst(const llvm::Instruction &inst);
  void translate_type(const llvm::Type *ty);
  void translate_value(const llvm::Value *v);

private:
  const llvm::Module &mod;
  Builder &builder;
};

Writer::Writer(const llvm::Module &in_mod, Builder &in_builder)
    : mod(in_mod), builder(in_builder) {}

// @see
// https://github.com/KhronosGroup/SPIRV-LLVM-Translator/blob/master/lib/SPIRV/SPIRVWriter.cpp:4072
void Writer::generate() {
  extract_meta();

  // llvm.constr
  for (auto &gv : mod.globals()) {
    std::string name = (std::string)gv.getName();
    printf("variable: %s\n", name.c_str());
#if !defined(NDEBUG)
    gv.dump();
#endif
  }

  printf("********* structTypes:\n");
  // %struct._depth_2d_t.191 = type opaque
  auto types = mod.getIdentifiedStructTypes();
  for (auto &ty : types) {
    // struct._depth_2d_t.191
    auto name = (std::string)ty->getStructName();
    auto tname = (std::string)ty->getName();
    // auto tname = (std::string)ty->getTypeByName();
#if !defined(NDEBUG)
    ty->dump();
#endif
  }

  printf("********* aliases:\n");
  for (auto &alias : mod.aliases()) {
#if !defined(NDEBUG)
    alias.dump();
#endif
  }

  printf("********* ifuns:\n");
  for (auto &ifun : mod.ifuncs()) {
#if !defined(NDEBUG)
    ifun.dump();
#endif
  }

  printf("********* named metadatas:\n");

  llvm::NamedMDNode *fn_list = nullptr;
  for (auto &meta : mod.named_metadata()) {
#if !defined(NDEBUG)
    meta.dump();
#endif
  }

  for (auto &func : mod) {
    auto func_type = func.getFunctionType();
    for (auto &attr : func.getAttributes()) {
    }
    std::string name = (std::string)func.getName();
    printf("function: %s\n", name.c_str());
    printf("arguments count: %d\n", func.arg_size());
    auto ty = func.getReturnType();
    for (auto &arg : func.args()) {
      std::string name = (std::string)arg.getName();
      auto argTy = arg.getType();
      argTy->isVoidTy();
      // auto addrspace = argTy->getPointerAddressSpace();
      arg.getValueName();
      // printf("\targument: %d %s\n", addrspace, name.c_str());
#if !defined(NDEBUG)
      arg.dump();
#endif
    }

    std::map<unsigned, Type *> ChangedType;
    if (func.isDeclaration()) {

    } else {
      // translateFunction
      translate(func);
    }
  }
}
void Writer::extract_meta() {
}

void Writer::translate(const Function &func) {
  bool is_intrin = func.isIntrinsic();
  auto name = func.getName();
  // todo: arguments, attributes
  for (auto &block : func) {

    for (auto &inst : block) {
      translate_inst(inst);
    }
  }
}

void Writer::translate_inst(const llvm::Instruction &inst) {
  // @see Instruction.def
  // Instruction::Add;
  auto code = inst.getOpcode();
  auto num = inst.getNumOperands();

  if (isa<LoadInst>(inst)) {
    printf("Load:\n\t");
  } else if (isa<StoreInst>(inst)) {
    printf("STORE:\n\t");
  } else if (isa<BinaryOperator>(inst)) {
    printf("Binary:\n\t");
  } else if (isa<ReturnInst>(inst)) {
    printf("Return:\n\t");
  } else if (isa<CmpInst>(inst)) {
    printf("Compare:\n\t");
  } else if (isa<SelectInst>(inst)) {
  } else if (isa<AllocaInst>(inst)) {
  } else if (isa<SwitchInst>(inst)) {
  } else if (isa<BranchInst>(inst)) {
    printf("BRANCH:\n\t");
  } else if (isa<PHINode>(inst)) {
    printf("**PHINode**:\n\t");
  } else if (isa<ExtractValueInst>(inst)) {
    printf("EXTRACT_VALUE:\n\t");
  } else if (isa<InsertValueInst>(inst)) {
    printf("INSERT_VALUE:\n\t");
  } else if (isa<UnaryInstruction>(inst)) {
  } else if (isa<GetElementPtrInst>(inst)) {
  } else if (isa<ExtractElementInst>(inst)) {
    printf("EXTRACT:\n\t");
  } else if (isa<InsertElementInst>(inst)) {
    printf("INSERT:\n\t");
  } else if (isa<ShuffleVectorInst>(inst)) {
    printf("Swizzle:\n\t");
  } else if (isa<AtomicRMWInst>(inst)) {
  } else if (isa<IntrinsicInst>(inst)) {
    printf("Intrin:\n\t");
  } else if (isa<FenceInst>(inst)) {

  } else if (isa<CallInst>(inst)) {
    const CallInst &call = *dyn_cast<CallInst>(&inst);
    call.args();
    call.getReturnedArgOperand();
    Function *func = call.getCalledFunction();

    auto name = (std::string)func->getName();
    printf("Call %s:\n\t", name.c_str());
  } else {
  }
  // inst.getOperand(0);

#if !defined(NDEBUG)
  inst.dump();
#endif
}

void Writer::translate_type(const Type *ty) {
  ty->isVoidTy();

  ty->isIntegerTy(1); // bool
  ty->isIntegerTy();
  ty->getIntegerBitWidth();

  ty->isFloatingPointTy();
  ty->getPrimitiveSizeInBits();

  ty->isTokenTy();

  ty->isPointerTy();
  auto ety = ty->getPointerElementType();
  /*

  */
  ty->getPointerAddressSpace();

  auto sty = dyn_cast<StructType>(ety);
  if (sty && !sty->isSized()) {
    // texture / sampler
  }
  sty->isOpaque();

  sty->hasName();
  int n = sty->getNumElements();
  if (n > 0) {
    sty->getElementType(0 /*n*/);
  }

  if (auto *ty_vec = dyn_cast<FixedVectorType>(ty)) {
  }

  ty->isArrayTy();
  ty->getArrayNumElements();

  ty->getArrayElementType();

  if (const FunctionType *tyfn = dyn_cast<FunctionType>(ty)) {
    tyfn->getReturnType();
    tyfn->params();
  }
}

void Writer::translate_value(const Value *v) {
  if (auto lbb = dyn_cast<BasicBlock>(v)) {
  }

  if (auto *f = dyn_cast<Function>(v)) {
  }

  if (auto gv = dyn_cast<GlobalVariable>(v)) {
  }

  if (isa<Constant>(v)) {
  }

  if (auto arg = dyn_cast<Argument>(v)) {
  }

  // begin instructions
  if (const StoreInst *st = dyn_cast<StoreInst>(v)) {
  }

  if (const LoadInst *lt = dyn_cast<LoadInst>(v)) {
  }

  if (const BinaryOperator *bi = dyn_cast<BinaryOperator>(v)) {
  }

  if (dyn_cast<UnreachableInst>(v)) {
  }

  if (auto ri = dyn_cast<ReturnInst>(v)) {
  }

  if (auto cmp = dyn_cast<CmpInst>(v)) {
  }

  if (auto sel = dyn_cast<SelectInst>(v)) {
  }

  if (auto alc = dyn_cast<AllocaInst>(v)) {
  }

  if (auto swt = dyn_cast<SwitchInst>(v)) {
  }

  if (auto br = dyn_cast<BranchInst>(v)) {
  }

  if (auto phi = dyn_cast<PHINode>(v)) {
  }

  if (auto evi = dyn_cast<ExtractValueInst>(v)) {
  }
  if (auto isi = dyn_cast<InsertValueInst>(v)) {
  }
  if (auto ui = dyn_cast<UnaryInstruction>(v)) {
  }

  if (auto gpi = dyn_cast<GetElementPtrInst>(v)) {
  }
  if (auto eei = dyn_cast<ExtractElementInst>(v)) {
  }
  if (auto iei = dyn_cast<InsertElementInst>(v)) {
  }
  if (auto svi = dyn_cast<ShuffleVectorInst>(v)) {
  }
  if (auto ai = dyn_cast<AtomicRMWInst>(v)) {
  }

  if (auto ii = dyn_cast<IntrinsicInst>(v)) {
  }

  if (auto ii = dyn_cast<FenceInst>(v)) {
  }

  if (auto ii = dyn_cast<CallInst>(v)) {
  }
  if (auto ii = dyn_cast<llvm::Instruction>(v)) {
  }
  llvm_unreachable("Not implemented");
}

/*
spv_ostream &operator<<(spv_ostream &O, SPIRVModule &M) {
  SPIRVModuleImpl &MI = *static_cast<SPIRVModuleImpl *>(&M);
  // Start tracking of the current line with no line
  MI.CurrentLine.reset();

  SPIRVEncoder Encoder(O);
  Encoder << MagicNumber << MI.SPIRVVersion
          << (((SPIRVWord)MI.GeneratorId << 16) | MI.GeneratorVer)
          << MI.NextId
          << MI.InstSchema;
  O << SPIRVNL();

  for (auto &I : MI.CapMap)
    O << *I.second;

  for (auto &I : M.getExtension()) {
    assert(!I.empty() && "Invalid extension");
    O << SPIRVExtension(&M, I);
  }

  for (auto &I : MI.IdToInstSetMap)
    O << SPIRVExtInstImport(&M, I.first, SPIRVBuiltinSetNameMap::map(I.second));

  O << SPIRVMemoryModel(&M);

  O << MI.EntryPointVec;

  for (auto &I : MI.EntryPointVec)
    MI.get<SPIRVFunction>(I->getTargetId())->encodeExecutionModes(O);

  O << MI.StringVec;

  for (auto &I : M.getSourceExtension()) {
    assert(!I.empty() && "Invalid source extension");
    O << SPIRVSourceExtension(&M, I);
  }

  O << SPIRVSource(&M);

  for (auto &I : MI.NamedId) {
    // Don't output name for entry point since it is redundant
    bool IsEntryPoint = false;
    for (auto &EPS : MI.EntryPointSet)
      if (EPS.second.count(I)) {
        IsEntryPoint = true;
        break;
      }
    if (!IsEntryPoint)
      M.getEntry(I)->encodeName(O);
  }

  if (M.isAllowedToUseExtension(
          ExtensionID::SPV_INTEL_memory_access_aliasing)) {
    O << SPIRVNL() << MI.AliasInstMDVec;
  }

  TopologicalSort TS(MI.TypeVec, MI.ConstVec, MI.VariableVec,
                     MI.ForwardPointerVec);

  O << MI.MemberNameVec << MI.ModuleProcessedVec << MI.DecGroupVec
    << MI.DecorateVec << MI.GroupDecVec << MI.ForwardPointerVec << TS;

  if (M.isAllowedToUseExtension(ExtensionID::SPV_INTEL_inline_assembly)) {
    O << SPIRVNL() << MI.AsmTargetVec << MI.AsmVec;
  }

  O << SPIRVNL() << MI.DebugInstVec << SPIRVNL() << MI.FuncVec;
  return O;
  }
*/
} // namespace spv

namespace metal {
using namespace spv;

void Library::Function::parse_arguments() {
  llvm::NamedMDNode *fn_list = nullptr;
  switch (shader_type) {
  case MTL2SPV_SHADER_VERTEX: {
    fn_list = module->getNamedMetadata("air.vertex");
    break;
  }
  case MTL2SPV_SHADER_FRAGMENT: {
    fn_list = module->getNamedMetadata("air.fragment");
    break;
  }
  case MTL2SPV_SHADER_COMPUTE: {
    fn_list = module->getNamedMetadata("air.kernel");
    break;
  }
  default:
    break;
  }
  if (fn_list) {
    assert(fn_list->getNumOperands() == 1);
    MDNode *root = fn_list->getOperand(0);
    int operand_count = root->getNumOperands();
    assert(operand_count >= 3);
    // first is function
    // second is ??
    // third is args info
    MDNode *node = dyn_cast_or_null<MDNode>(root->getOperand(2 /*arg_infos*/));
    // TODO: use arg match parser..
    for (auto &arg_parent : node->operands()) {
      mtl2spv_argument arg;
      MDNode *node = dyn_cast_or_null<MDNode>(arg_parent);
      assert(node && node->getNumOperands() > 3);
      auto param_id = mdconst::dyn_extract<ConstantInt>(node->getOperand(0))
                          ->getZExtValue();
      arg.index = (int)param_id;
      auto storage_type =
          dyn_cast_or_null<MDString>(node->getOperand(1))->getString().str();
      if (storage_type == "air.texture") {            //  tex id at level, element count
        // !{i32 1, !"air.texture", !"air.location_index", i32 1, i32 1, !"air.write",      // air.sample, air.read_write
        //  !"air.arg_type_name", !"texture2d<float, write>", !"air.arg_name", !"outDepth"}
        arg.type = MTL2SPV_ARGUMENT_TYPE_TEXTURE;
        auto location_index_str =
            dyn_cast_or_null<MDString>(node->getOperand(2))->getString().str();
        auto location_index =
            mdconst::dyn_extract<ConstantInt>(node->getOperand(3))
                ->getZExtValue();
        arg.index = (int)location_index;
        auto location_range =
            mdconst::dyn_extract<ConstantInt>(node->getOperand(4))
                ->getZExtValue();
        auto access_str =
            dyn_cast_or_null<MDString>(node->getOperand(5))->getString().str();
        auto arg_type_str =
            dyn_cast_or_null<MDString>(node->getOperand(6))->getString().str();
        auto arg_type =
            dyn_cast_or_null<MDString>(node->getOperand(7))->getString().str();
        auto arg_name_str =
            dyn_cast_or_null<MDString>(node->getOperand(8))->getString().str();
        auto arg_name =
            dyn_cast_or_null<MDString>(node->getOperand(9))->getString().str();
        //arg.name = arg_name;

      } else if (storage_type == "air.buffer") {  // or air.indirect_buffer      // buf_id at level, buf array size, (how many buffers)
          //!{ i32 2, !"air.buffer", !"air.buffer_size", i32 16, !"air.location_index", i32 2, i32 1, !"air.read", or air.read_write
          //   !"air.arg_type_size", i32 16, !"air.arg_type_align_size", i32 16, !"air.arg_type_name", !"uint4", !"air.arg_name", !"inputRect"}
        arg.type = MTL2SPV_ARGUMENT_TYPE_BUFFER;
        auto buffer_size_str =
            dyn_cast_or_null<MDString>(node->getOperand(2))->getString().str();
        auto buffer_size =
            mdconst::dyn_extract<ConstantInt>(node->getOperand(3))
                ->getZExtValue();
        // TODO: air.struct_type_info
      } else if (storage_type == "air.thread_position_in_grid") {
          // i32 3, !"air.thread_position_in_grid", !"air.arg_type_name", !"uint2", !"air.arg_name", !"tid"
      } else if (storage_type == "air.indirect_constant") {
      } else {
      // graphics stage input:
          // air.position, air.center, air.no_perspective
          // air.patch_control_point_input
      }
      // air.fragment_input
      // air.instance_id
      // air.vertex_id
      // air.point_size
      // air.vertex_output
      // air.patch
      // air.primitive_id
      // air.center
      // air.perspective
      // air.barycentric_coord
      // air.point_coord
      // air.render_target
      // air.depth
      // air.depth_qualifier, air.any[less,greater]

    }
  }
}

void Library::Function::write_spirv(const mtl2spv_options &options,
                                    spv::Blob &out_spirv) const {
  Builder builder(options, entry_name, shader_type, out_spirv);
  Writer writer(*module, builder);
  writer.generate();
}

void Library::write_spv(const mtl2spv_options &options, int index,
                        spv::Blob &out_spirv) const {
  if (index < (int)functions.size()) {
    functions[index].write_spirv(options, out_spirv);
  }
}

void Library::write_spv(const mtl2spv_options &options, const String &name,
                        spv::Blob &out_spirv) const {
  auto iter = map.find(name);
  if (iter != map.end()) {
    write_spv(options, (int)iter->second, out_spirv);
  }
}
} // namespace metal
