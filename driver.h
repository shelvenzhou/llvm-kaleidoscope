#ifndef LLVM_KALEIDOSCOPE_DRIVER_H_
#define LLVM_KALEIDOSCOPE_DRIVER_H_

#include "lexer.h"
#include "parser.h"

#include "stdio.h"

namespace driver {

static void handle_definition();
static void handle_extern();
static void handle_top_level_expression();

static void main_loop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch(parser::current_token) {
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

#endif // LLVM_KALEIDOSCOPE_DRIVER_H_
