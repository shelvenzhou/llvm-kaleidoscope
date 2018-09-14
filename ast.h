#ifndef LLVM_KALEIDOSCOPE_AST_H_
#define LLVM_KALEIDOSCOPE_AST_H_

#include <string>
#include <memory>

namespace ast {

class ExprAST {
public:
    virtual ~ExprAST() = default;
};

class NumberExprAst: public ExprAST {
    double val_;

public:
    NumberExprAst(double val): val_(val) {}
};

class VariableExprAST: public ExprAST {
    std::string name_;

public:
    VariableExprAST(const std::string &name): name_(name) {}
};

class BinaryExprAST: public ExprAST {
    char op_;
    std::unique_ptr<ExprAST> lhs_, rhs_;

public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
        : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
};

class CallExprAST: public ExprAST {
    std::string callee_;
    std::vector<std::unique_ptr<ExprAST>> args_;

public:
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
        : callee_(callee), args_(std::move(args)) {}
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
};

// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> proto_;
    std::unique_ptr<ExprAST> body_;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
        : proto_(std::move(proto)), body_(std::move(body)) {}
};

} // namespace ast

#endif // LLVM_KALEIDOSCOPE_AST_H_
