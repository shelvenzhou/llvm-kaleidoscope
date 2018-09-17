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
        switch(current_token) {
            case kTokenEof:
                return;
            case ';':
                get_next_token();
                break;
            case kTokenDef:
                handle_definition();
                break;
            case kTokenExtern:
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
