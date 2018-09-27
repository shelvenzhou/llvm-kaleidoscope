#include "ast.h"
#include "parser_log.h"

#include <memory>
#include <map>
#include <vector>
#include <string>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

namespace ast {

llvm::LLVMContext llvm_context;
llvm::IRBuilder<> ir_builder(llvm_context);
std::unique_ptr<llvm::Module> module;
std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
std::map<std::string, llvm::Value *> name_values;

void initialize_module_and_pass_manager() {
    module = llvm::make_unique<llvm::Module>("module", llvm_context);

    function_pass_manager = llvm::make_unique<llvm::legacy::FunctionPassManager>(module.get());

    // do simple "peephole" optimizations and bit-twiddling optzns
    function_pass_manager->add(llvm::createInstructionCombiningPass());
    // reassociate expressions
    function_pass_manager->add(llvm::createReassociatePass());
    // eliminate common sub-expressions
    function_pass_manager->add(llvm::createGVNPass());
    // simplify the control flow graph (deleting unreachable blocks, etc)
    function_pass_manager->add(llvm::createCFGSimplificationPass());

    function_pass_manager->doInitialization();
}

void print_generated_code() {
    module->print(llvm::errs(), nullptr);
}

llvm::Value *NumberExprAst::codegen() {
    return llvm::ConstantFP::get(llvm_context, llvm::APFloat(val_));
}

llvm::Value *VariableExprAST::codegen() {
    llvm::Value *v = name_values[name_];
    if (!v)
        parser_log::log_error_value("unknown variable name");
    return v;
}

llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *l = lhs_->codegen();
    llvm::Value *r = rhs_->codegen();

    if (!l || !r)
        return nullptr;

    switch (op_) {
        case '+':
            return ir_builder.CreateFAdd(l, r, "addtmp");
        case '-':
            return ir_builder.CreateFSub(l, r, "subtmp");
        case '*':
            return ir_builder.CreateFMul(l, r, "multmp");
        case '<':
            l = ir_builder.CreateFCmpULT(l, r, "cmptmp");
            return ir_builder.CreateUIToFP(l, llvm::Type::getDoubleTy(llvm_context), "booltmp");
        default:
            return parser_log::log_error_value("invalid binary operator");
    }
}

llvm::Value *CallExprAST::codegen() {
    llvm::Function *callee = module->getFunction(callee_);
    if (!callee)
        parser_log::log_error_value("unknown function referenced");

    if (callee->arg_size() != args_.size())
        return parser_log::log_error_value("incorrect # arguments passed");

    std::vector<llvm::Value *> args;
    for (auto &arg : args_) {
        args.push_back(arg->codegen());
        if (!args.back())
            return nullptr;
    }

    return ir_builder.CreateCall(callee, args, "calltmp");
}

llvm::Function *PrototypeAST::codegen() {
    std::vector<llvm::Type *> arg_types(args_.size(), llvm::Type::getDoubleTy(llvm_context));
    llvm::FunctionType *function_type = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm_context), arg_types, false);

    llvm::Function *function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, name_, module.get());

    unsigned i = 0;
    for (auto& arg : function->args())
        arg.setName(args_[i++]);

    return function;
}

llvm::Function *FunctionAST::codegen() {
    llvm::Function *function = module->getFunction(proto_->name());

    if (!function)
        function = proto_->codegen();

    if (!function)
        return nullptr;

    if (!function->empty())
        return parser_log::log_error_function("function cannot be redefined");

    llvm::BasicBlock *bb = llvm::BasicBlock::Create(llvm_context, "entry", function);
    ir_builder.SetInsertPoint(bb);

    name_values.clear();
    for (auto &arg : function->args())
        name_values[arg.getName()] = &arg;

    if (llvm::Value *return_value = body_->codegen()) {
        ir_builder.CreateRet(return_value);

        llvm::verifyFunction(*function);

        function_pass_manager->run(*function);

        return function;
    }

    function->eraseFromParent();
    return nullptr;
}

} // namespace ast
