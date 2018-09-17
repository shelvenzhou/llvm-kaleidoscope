#ifndef LLVM_KALEIDOSCOPE_DRIVER_H_
#define LLVM_KALEIDOSCOPE_DRIVER_H_

namespace driver {

void handle_definition();
void handle_extern();
void handle_top_level_expression();

void main_loop();

} // namespace driver

#endif // LLVM_KALEIDOSCOPE_DRIVER_H_
