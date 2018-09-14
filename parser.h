#ifndef LLVM_KALEIDOSCOPE_PARSER_H_
#define LLVM_KALEIDOSCOPE_PARSER_H_

#include "ast.h"
#include "lexer.h"

#include <ctype.h>

#include <memory>
#include <map>

#include "llvm/ADT/STLExtras.h"

/*
Grammar of Kaleidoscope

    numberexpr := number

    parenexpr := '(' expression ')'

    identifierexpr := identifier
                   := identifier '(' expression* ')' // for function call

    primary := identifierexpr
            := numberexpr
            := parenexpr

    expression := primary binaryoperationrhs

    binaryoperationrhs := ('+'|'-'|'*'|'<' primary)*

    prototype := identifier '(' identifier* ')'

    definition := 'def' prototype expression

    external := 'extern' prototype

    top_level_expr := expression
*/

namespace parser {

static int current_token;
static int get_next_token() {
    return current_token = get_token();
}

static std::map<char, int> binary_operator_precedence = {
    { '<', 10 },
    { '+', 20 },
    { '-', 20 },
    { '*', 40 },
};
static int get_token_precedence() {
    if (!isascii(current_token))
        return -1;

    int token_precedence = binary_operator_precedence[current_token];
    if (token_precedence <= 0)
        return -1;
    return token_precedence;
}

static std::unique_ptr<ExprAST> parse_number_expr();
static std::unique_ptr<ExprAST> parse_paren_expr();
static std::unique_ptr<ExprAST> parse_identifier_expr();
static std::unique_ptr<ExprAST> parse_primary();
static std::unique_ptr<ExprAST> parse_binary_operation_rhs(int expr_precedence, std::unique_ptr<ExprAST> lhs);
static std::unique_ptr<ExprAST> parse_expression();
static std::unique_ptr<PrototypeAST> parse_prototype();
static std::unique_ptr<FunctionAST> parse_definition();
static std::unique_ptr<PrototypeAST> parse_external();
static std::unique_ptr<ExprAST> parse_top_level_expr();

} // namespace parser

#endif // LLVM_KALEIDOSCOPE_PARSER_H_
