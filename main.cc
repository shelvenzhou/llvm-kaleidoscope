#include "driver.h"
#include "parser.h"
#include "ast.h"

#include <stdio.h>

#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/STLExtras.h"

int main() {
    fprintf(stderr, "ready> ");
    parser::get_next_token();

    ast::module = llvm::make_unique<llvm::Module>("module", ast::llvm_context);

    driver::main_loop();

    ast::module->print(llvm::errs(), nullptr);

    return 0;
}
