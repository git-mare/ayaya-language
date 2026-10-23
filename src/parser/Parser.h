#pragma once
#include <vector>
#include <memory>
#include "../utils/Token.h"
#include "../ast/Node.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t pos;
    Token current_token;

    void advance();
    void error(const std::string& msg);
    std::shared_ptr<Node> expr();
    std::shared_ptr<Node> and_expr();
    std::shared_ptr<Node> comparison();
    std::shared_ptr<Node> additive();
    std::shared_ptr<Node> term();
    std::shared_ptr<Node> unary();
    std::shared_ptr<Node> factor();
    std::shared_ptr<Node> parse_statement();
    std::vector<std::shared_ptr<Node>> parse_block();

public:
    Parser(const std::vector<Token>& t);
    std::vector<std::shared_ptr<Node>> parse();
};
