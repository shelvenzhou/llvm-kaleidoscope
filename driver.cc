#include "driver.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

#include "llvm/Support/raw_ostream.h"

namespace driver {

void handle_definition() {
    if (auto definition_ast = parser::parse_definition()) {
        if (auto *definition_ir = definition_ast->codegen()) {
            fprintf(stderr, "Read function definition:");
            definition_ir->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        parser::get_next_token();
    }
}

void handle_extern() {
    if (auto external_ast = parser::parse_external()) {
        if (auto *external_ir = external_ast->codegen()) {
            fprintf(stderr, "Read extern:");
            external_ir->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        parser::get_next_token();
    }
}

void handle_top_level_expression() {
    if (auto expr_ast = parser::parse_top_level_expr()) {
        if (auto *expr_ir = expr_ast->codegen()) {
            fprintf(stderr, "Read top-level expression:");
            expr_ir->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        parser::get_next_token();
    }
}

void main_loop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (parser::current_token) {
            case lexer::kTokenEof:
                return;
            case ';':
                parser::get_next_token();
                break;
            case lexer::kTokenDef:
                handle_definition();
                break;
            case lexer::kTokenExtern:
                handle_extern();
                break;
            default:
                handle_top_level_expression();
                break;
        }
    }
}

} // namespace driver
