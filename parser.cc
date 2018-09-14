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

static std::unique_ptr<ExprAST> parse_binary_operation_rhs(int expr_precedence, std::unique_ptr<ExprAST> lhs);
static std::unique_ptr<ExprAST> parse_expression();
static std::unique_ptr<PrototypeAST> parse_prototype();
static std::unique_ptr<FunctionAST> parse_definition();
static std::unique_ptr<PrototypeAST> parse_external();
static std::unique_ptr<ExprAST> parse_top_level_expr();

} // namespace parser
