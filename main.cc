#include "driver.h"
#include "parser.h"
#include "ast.h"

#include <stdio.h>

int main() {
    fprintf(stderr, "ready> ");
    parser::get_next_token();

    ast::initialize_module_and_pass_manager();

    driver::main_loop();

    ast::print_generated_code();

    return 0;
}
