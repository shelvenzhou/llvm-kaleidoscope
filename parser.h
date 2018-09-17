#ifndef LLVM_KALEIDOSCOPE_PARSER_H_
#define LLVM_KALEIDOSCOPE_PARSER_H_

#include "ast.h"

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
int get_next_token();

static std::map<char, int> binary_operator_precedence = {
    { '<', 10 },
    { '+', 20 },
    { '-', 20 },
    { '*', 40 },
};
int get_token_precedence();

std::unique_ptr<ast::ExprAST> parse_number_expr();
std::unique_ptr<ast::ExprAST> parse_paren_expr();
std::unique_ptr<ast::ExprAST> parse_identifier_expr();
std::unique_ptr<ast::ExprAST> parse_primary();
std::unique_ptr<ast::ExprAST> parse_binary_operation_rhs(int expr_precedence, std::unique_ptr<ast::ExprAST> lhs);
std::unique_ptr<ast::ExprAST> parse_expression();
std::unique_ptr<ast::PrototypeAST> parse_prototype();
std::unique_ptr<ast::FunctionAST> parse_definition();
std::unique_ptr<ast::PrototypeAST> parse_external();
std::unique_ptr<ast::FunctionAST> parse_top_level_expr();

} // namespace parser

#endif // LLVM_KALEIDOSCOPE_PARSER_H_
