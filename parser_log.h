#ifndef LLVM_KALEIDOSCOPE_PARSER_LOG_H_
#define LLVM_KALEIDOSCOPE_PARSER_LOG_H_

#include "ast.h"

#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"

namespace parser_log {

std::unique_ptr<ast::ExprAST> log_error(const char *error);
std::unique_ptr<ast::PrototypeAST> log_error_prototype(const char *error);
llvm::Value *log_error_value(const char *error);
llvm::Function *log_error_function(const char *error);

} // namespace log

#endif // LLVM_KALEIDOSCOPE_PARSER_LOG_H_
