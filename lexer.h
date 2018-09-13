#ifndef LLVM_KALEIDOSCOPE_LEXER_H_
#define LLVM_KALEIDOSCOPE_LEXER_H_

#include <string>

namespace kaleidoscope {
namespace lexer {

enum Token {
    kTokenEof = -1,

    kTokenDef = -2,
    kTokenExtern = -3,

    kTokenIdentifier = -4,
    kTokenNumber = -5,
};

static std::string identifier_str;
static double num_val;

static int get_token();

} // namespace lexer
} // namespace kaleidoscope

#endif // LLVM_KALEIDOSCOPE_LEXER_H_
