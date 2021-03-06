#ifndef LLVM_KALEIDOSCOPE_LEXER_H_
#define LLVM_KALEIDOSCOPE_LEXER_H_

#include <string>

/*
Token of Kaleidoscope

    identifier: [a-zA-Z][a-zA-Z0-9]*

    number: [0-9.]+
*/

namespace lexer {

enum Token {
    kTokenEof = -1,

    kTokenDef = -2,
    kTokenExtern = -3,

    kTokenIdentifier = -4,
    kTokenNumber = -5,
};

extern std::string identifier_str;
extern double num_val;

int get_token();

} // namespace lexer

#endif // LLVM_KALEIDOSCOPE_LEXER_H_
