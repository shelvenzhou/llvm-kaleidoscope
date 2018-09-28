#include "driver.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

#include <stdio.h>

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Error.h"

namespace driver {

void handle_definition() {
    if (auto definition_ast = parser::parse_definition()) {
        if (auto *definition_ir = definition_ast->codegen()) {
            fprintf(stderr, "Read function definition:");
            definition_ir->print(llvm::errs());
            fprintf(stderr, "\n");

            ast::jit_engine->addModule(std::move(ast::module));
            ast::initialize_module_and_pass_manager();
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

            ast::function_protos[external_ast->name()] = std::move(external_ast);
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

            auto handler = ast::jit_engine->addModule(std::move(ast::module));
            ast::initialize_module_and_pass_manager();

            auto expr_symbol = ast::jit_engine->findSymbol("__anon_expr");
            assert(expr_symbol && "Function not found");

            double (*function_pointer)() = (double (*)())(intptr_t)llvm::cantFail(expr_symbol.getAddress());
            fprintf(stderr, "Evaluated to %f\n", function_pointer());

            ast::jit_engine->removeModule(handler);
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
