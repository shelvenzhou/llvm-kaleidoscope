#include "ast.h"

#include <stdio.h>

#include <memory>

#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"

namespace parser_log {

std::unique_ptr<ast::ExprAST> log_error(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
    return nullptr;
}

std::unique_ptr<ast::PrototypeAST> log_error_prototype(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
    return nullptr;
}

llvm::Value *log_error_value(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
    return nullptr;
}

llvm::Function *log_error_function(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
    return nullptr;
}

} // namespace log
