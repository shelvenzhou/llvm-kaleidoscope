#include "driver.h"
#include "parser.h"

#include <stdio.h>

int main() {
    fprintf(stderr, "ready> ");
    get_next_token();

    main_loop();

    return 0;
}
