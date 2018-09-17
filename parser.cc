#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "parser_log.h"

#include <memory>
#include <string>
#include <vector>

namespace parser {

static std::unique_ptr<ExprAST> parse_number_expr() {
    auto result = llvm::make_unique<NumberExprAst>(num_val);
    get_next_token();
    return std::move(result);
}

static std::unique_ptr<ExprAST> parse_paren_expr() {
    get_next_token(); // eat '('
    auto expr = parse_expression();
    if (!expr)
        return nullptr;

    if (current_token != ')')
        return log_error("expected ')'");
    get_next_token();
    return expr;
}

static std::unique_ptr<ExprAST> parse_identifier_expr() {
    std::string identifier = identifier_str;

    get_next_token();

    if (current_token != ')')
        return llvm::make_unique<VariableExprAST>(identifier);

    // function call
    get_next_token();
    std::vector<std::unique_ptr<ExprAST>> args;
    if (current_token != ')') {
        while (true) {
            if (auto arg = parse_expression())
                args.push_back(arg);
            else
                return nullptr;

            if (current_token == ')')
                break;

            if (current_token != ',')
                return log_error("expected ',' or ')' in argument list");
            get_next_token();
        }
    }

    get_next_token();
    return llvm::make_unique<CallExprAST>(identifier, std::move(args));
}

static std::unique_ptr<ExprAST> parse_primary() {
    switch (current_token) {
        case kTokenIdentifier:
            return parse_identifier_expr();
        case kTokenNumber:
            return parse_number_expr();
        case '(':
            return parse_paren_expr();
        default:
            return log_error("unknown token when expecting an expression");
    }
}


static std::unique_ptr<ExprAST> parse_binary_operation_rhs(int expr_precedence, std::unique_ptr<ExprAST> lhs) {
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

        lhs = llvm::make_unique<BinaryExprAST>(binary_operator, std::move(lhs), std::move(rhs));
    }
}

static std::unique_ptr<ExprAST> parse_expression() {
    auto lhs = parse_primary();
    if (!lhs)
        return nullptr;

    return parse_binary_operation_rhs(0, std::move(lhs));
}


static std::unique_ptr<PrototypeAST> parse_prototype();
static std::unique_ptr<FunctionAST> parse_definition();
static std::unique_ptr<PrototypeAST> parse_external();
static std::unique_ptr<ExprAST> parse_top_level_expr();

} // namespace parser
