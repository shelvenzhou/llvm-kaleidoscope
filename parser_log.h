#ifndef LLVM_KALEIDOSCOPE_PARSER_LOG_H_
#define LLVM_KALEIDOSCOPE_PARSER_LOG_H_

#include "ast.h"

#include <stdio.h>

#include <memory>

namespace log {

std::unique_ptr<ExprAST> log_error(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
    return nullptr;
}

std::unique_ptr<PrototypeAST> log_error_prototype(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
    return nullptr;
}

} // namespace log

#endif // LLVM_KALEIDOSCOPE_PARSER_LOG_H_
