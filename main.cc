#include "driver.h"
#include "parser.h"
#include "ast.h"

#include <stdio.h>

int main() {
    fprintf(stderr, "ready> ");
    parser::get_next_token();

    ast::initialize_jit();
    ast::initialize_module_and_pass_manager();

    driver::main_loop();

    return 0;
}
