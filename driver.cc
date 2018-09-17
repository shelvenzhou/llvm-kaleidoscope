#include "driver.h"
#include "parser.h"

#include <stdio.h>

namespace driver {

static void handle_definition() {
    if (parse_definition())
        fprintf(stderr, "Parsed a function definition.\n");
    else
        get_next_token();
}

static void handle_extern() {
    if (parse_external())
        fprintf(stderr, "Parsed an extern.\n");
    else
        get_next_token();
}

static void handle_top_level_expression() {
    if (parse_top_level_expr())
        fprintf(stderr, "Parsed a top-level expression.\n");
    else
        get_next_token();
}

} // namespace driver
