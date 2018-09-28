#ifndef LLVM_KALEIDOSCOPE_AST_H_
#define LLVM_KALEIDOSCOPE_AST_H_

#include <string>
#include <memory>
#include <vector>
#include <map>

#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "KaleidoscopeJIT.h"

namespace ast {

extern std::unique_ptr<llvm::Module> module;
extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> jit_engine;

void initialize_jit();
void initialize_module_and_pass_manager();
void print_generated_code();

class ExprAST {
public:
    virtual ~ExprAST() = default;

    virtual llvm::Value *codegen() = 0;
};

class NumberExprAst: public ExprAST {
    double val_;

public:
    NumberExprAst(double val): val_(val) {}

    virtual llvm::Value *codegen();
};

class VariableExprAST: public ExprAST {
    std::string name_;

public:
    VariableExprAST(const std::string &name): name_(name) {}

    virtual llvm::Value *codegen();
};

class BinaryExprAST: public ExprAST {
    char op_;
    std::unique_ptr<ExprAST> lhs_, rhs_;

public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
        : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    virtual llvm::Value *codegen();
};

class CallExprAST: public ExprAST {
    std::string callee_;
    std::vector<std::unique_ptr<ExprAST>> args_;

public:
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
        : callee_(callee), args_(std::move(args)) {}

    virtual llvm::Value *codegen();
};

// PrototypeAST - This class represents the "prototype" for a function,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
class PrototypeAST {
    std::string name_;
    std::vector<std::string> args_;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> args)
        : name_(name), args_(std::move(args)) {}

    const std::string &name() const { return name_; }

    llvm::Function *codegen();
};

// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> proto_;
    std::unique_ptr<ExprAST> body_;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
        : proto_(std::move(proto)), body_(std::move(body)) {}

    llvm::Function *codegen();
};

extern std::map<std::string, std::unique_ptr<ast::PrototypeAST>> function_protos;

} // namespace ast

#endif // LLVM_KALEIDOSCOPE_AST_H_
