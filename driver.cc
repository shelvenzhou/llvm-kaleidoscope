#include "driver.h"
#include "parser.h"

#include <stdio.h>

namespace driver {

static void handle_definition() {
    if (parser::parse_definition())
        fprintf(stderr, "Parsed a function definition.\n");
    else
        parser::get_next_token();
}

static void handle_extern() {
    if (parser::parse_external())
        fprintf(stderr, "Parsed an extern.\n");
    else
        parser::get_next_token();
}

static void handle_top_level_expression() {
    if (parser::parse_top_level_expr())
        fprintf(stderr, "Parsed a top-level expression.\n");
    else
        parser::get_next_token();
}

} // namespace driver
