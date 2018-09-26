#include "driver.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

namespace driver {

void handle_definition() {
    if (parser::parse_definition())
        fprintf(stderr, "Parsed a function definition.\n");
    else
        parser::get_next_token();
}

void handle_extern() {
    if (parser::parse_external())
        fprintf(stderr, "Parsed an extern.\n");
    else
        parser::get_next_token();
}

void handle_top_level_expression() {
    if (parser::parse_top_level_expr())
        fprintf(stderr, "Parsed a top-level expression.\n");
    else
        parser::get_next_token();
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
