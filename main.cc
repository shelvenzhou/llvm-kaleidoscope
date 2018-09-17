#include "driver.h"
#include "parser.h"

#include <stdio.h>

int main() {
    fprintf(stderr, "ready> ");
    parser::get_next_token();

    driver::main_loop();

    return 0;
}
