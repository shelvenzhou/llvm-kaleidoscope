#include "lexer.h"

#include <stdio.h>
#include <ctype.h>

#include <string>

namespace kaleidoscope {
namespace lexer {

static int get_token() {
    static int last_char = ' ';

    while (isspace(last_char))
        last_char = getchar();

    if (isalpha(last_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifier_str = last_char;
        while (isalnum(last_char = getchar()))
            identifier_str += last_char;

        if (identifier_str == "def")
            return kTokenDef;
        if (identifier_str == "extern")
            return kTokenExtern;
        return kTokenIdentifier;
    }

    if (isdigit(last_char) || last_char == '.') { // number: [0-9.]+
        std::string num_str;
        do {
            num_str += last_char;
            last_char = getchar();
        } while (isdigit(last_char) || last_char == '.');

        num_val = strtod(num_str.c_str(), 0);
        return kTokenNumber;
    }

    if (last_char == '#') {
        // comment until end of line
        do
            last_char = getchar();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF)
            return get_token();
    }

    if (last_char == EOF)
        return kTokenEof;

    int this_char = last_char;
    last_char = getchar();
    return this_char;
}

} // namespace lexer
} // namespace kaleidoscope
