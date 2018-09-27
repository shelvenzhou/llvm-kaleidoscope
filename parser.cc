#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "parser_log.h"

#include <ctype.h>

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "llvm/ADT/STLExtras.h"

namespace parser {

int current_token;
int get_next_token() {
    return current_token = lexer::get_token();
}

std::map<char, int> binary_operator_precedence = {
    { '<', 10 },
    { '+', 20 },
    { '-', 20 },
    { '*', 40 },
};
int get_token_precedence() {
    if (!isascii(current_token))
        return -1;

    int token_precedence = binary_operator_precedence[current_token];
    if (token_precedence <= 0)
        return -1;
    return token_precedence;
}

std::unique_ptr<ast::ExprAST> parse_number_expr() {
    auto result = llvm::make_unique<ast::NumberExprAst>(lexer::num_val);
    get_next_token();
    return std::move(result);
}

std::unique_ptr<ast::ExprAST> parse_paren_expr() {
    get_next_token(); // eat '('
    auto expr = parse_expression();
    if (!expr)
        return nullptr;

    if (current_token != ')')
        return parser_log::log_error("expected ')'");
    get_next_token();
    return expr;
}

std::unique_ptr<ast::ExprAST> parse_identifier_expr() {
    std::string identifier = lexer::identifier_str;

    get_next_token();

    if (current_token != '(')
        return llvm::make_unique<ast::VariableExprAST>(identifier);

    // function call
    get_next_token();
    std::vector<std::unique_ptr<ast::ExprAST>> args;
    if (current_token != ')') {
        while (true) {
            if (auto arg = parse_expression())
                args.push_back(std::move(arg));
            else
                return nullptr;

            if (current_token == ')')
                break;

            if (current_token != ',')
                return parser_log::log_error("expected ',' or ')' in argument list");
            get_next_token();
        }
    }

    get_next_token();
    return llvm::make_unique<ast::CallExprAST>(identifier, std::move(args));
}

std::unique_ptr<ast::ExprAST> parse_primary() {
    switch (current_token) {
        case lexer::kTokenIdentifier:
            return parse_identifier_expr();
        case lexer::kTokenNumber:
            return parse_number_expr();
        case '(':
            return parse_paren_expr();
        default:
            return parser_log::log_error("unknown token when expecting an expression");
    }
}


std::unique_ptr<ast::ExprAST> parse_binary_operation_rhs(int expr_precedence, std::unique_ptr<ast::ExprAST> lhs) {
    while (true) {
        int token_precedence = get_token_precedence();

        // expr_precedence indicates the minimal operator precedence that the function is allowed to eat
        // since we define the precedence of invalid tokens to be -1, they will always cause an immediate return
        if (token_precedence < expr_precedence)
            return lhs;

        int binary_operator = current_token;
        get_next_token();

        auto rhs = parse_primary();
        if (!rhs)
            return nullptr;

        int next_precedence = get_token_precedence();
        if (token_precedence < next_precedence) {
            // current precedence is lower than the next one, parse the next on first
            // set the expr_precedence to filter out the operator with lower precedence
            rhs = parse_binary_operation_rhs(token_precedence+1, std::move(rhs));
            if (!rhs)
                return nullptr;
        }

        lhs = llvm::make_unique<ast::BinaryExprAST>(binary_operator, std::move(lhs), std::move(rhs));
    }
}

std::unique_ptr<ast::ExprAST> parse_expression() {
    auto lhs = parse_primary();
    if (!lhs)
        return nullptr;

    return parse_binary_operation_rhs(0, std::move(lhs));
}


std::unique_ptr<ast::PrototypeAST> parse_prototype() {
    if (current_token != lexer::kTokenIdentifier)
        parser_log::log_error_prototype("expected function name in prototype");

    std::string function_name = lexer::identifier_str;
    get_next_token();

    if (current_token != '(')
        return parser_log::log_error_prototype("expected '(' in prototype");

    std::vector<std::string> arg_names;
    while(get_next_token() == lexer::kTokenIdentifier)
        arg_names.push_back(lexer::identifier_str);
    if (current_token != ')')
        return parser_log::log_error_prototype("expected ')' in prototype");

    get_next_token();

    return llvm::make_unique<ast::PrototypeAST>(function_name, std::move(arg_names));
}

std::unique_ptr<ast::FunctionAST> parse_definition() {
    get_next_token();

    auto prototype = parse_prototype();
    if (!prototype)
        return nullptr;

    if (auto expression = parse_expression())
        return llvm::make_unique<ast::FunctionAST>(std::move(prototype), std::move(expression));

    return nullptr;
}

std::unique_ptr<ast::PrototypeAST> parse_external() {
    get_next_token();
    return parse_prototype();
}

std::unique_ptr<ast::FunctionAST> parse_top_level_expr() {
    if (auto expression = parse_expression()) {
        auto prototype = llvm::make_unique<ast::PrototypeAST>("__anon_expr", std::vector<std::string>());
        return llvm::make_unique<ast::FunctionAST>(std::move(prototype), std::move(expression));
    }
    return nullptr;
}

} // namespace parser
