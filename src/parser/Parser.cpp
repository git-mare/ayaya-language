#include "Parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& t) : tokens(t), pos(0), current_token(t[0]) {}

void Parser::advance() {
    pos++;
    if (pos < tokens.size()) current_token = tokens[pos];
}

std::shared_ptr<Node> Parser::factor() {
    if (current_token.type == TokenType::NUMBER) {
        int value = std::stoi(current_token.value);
        advance();
        return std::make_shared<NumberNode>(value);
    } else if (current_token.type == TokenType::IDENTIFIER) {
        std::string name = current_token.value;
        advance();
        return std::make_shared<VarNode>(name);
    } else if (current_token.type == TokenType::LPAREN) {
        advance();
        auto node = expr();
        if (current_token.type != TokenType::RPAREN)
            throw std::runtime_error("Expected )");
        advance();
        return node;
    }
    throw std::runtime_error("Invalid Factor");
}

std::shared_ptr<Node> Parser::term() {
    auto node = factor();
    while (current_token.type == TokenType::MUL || current_token.type == TokenType::DIV) {
        std::string op = current_token.value;
        advance();
        node = std::make_shared<BinOpNode>(node, op, factor());
    }
    return node;
}

std::shared_ptr<Node> Parser::expr() {
    auto node = term();

    // + and -
    while (current_token.type == TokenType::PLUS || current_token.type == TokenType::MINUS) {
        std::string op = current_token.value;
        advance();
        node = std::make_shared<BinOpNode>(node, op, term());
    }

    // >, <, >=, <=, ==, !=
    while (current_token.type == TokenType::GT || current_token.type == TokenType::LT ||
           current_token.type == TokenType::GE || current_token.type == TokenType::LE ||
           current_token.type == TokenType::EQ || current_token.type == TokenType::NE) {

        std::string op = current_token.value;
        advance();
        auto right = term();
        node = std::make_shared<BinOpNode>(node, op, right);
    }

    return node;
}

std::vector<std::shared_ptr<Node>> Parser::parse() {
    std::vector<std::shared_ptr<Node>> instructions;
    while (current_token.type != TokenType::END_OF_FILE) {
        instructions.push_back(parse_statement());
    }
    return instructions;
}

// Novo método para parse de uma instrução individual
std::shared_ptr<Node> Parser::parse_statement() {
    if (current_token.type == TokenType::IDENTIFIER) {
        std::string var_name = current_token.value;
        advance();
        if (current_token.type != TokenType::ASSIGN)
            throw std::runtime_error("Expected =");
        advance();
        auto value = expr();
        return std::make_shared<AssignNode>(var_name, value);

    } else if (current_token.type == TokenType::SHOW) {
        advance();
        if (current_token.type != TokenType::LPAREN)
            throw std::runtime_error("Expected ( after show");
        advance();
        auto value = expr();
        if (current_token.type != TokenType::RPAREN)
            throw std::runtime_error("Expected )");
        advance();
        return std::make_shared<ShowNode>(value);

    } else if (current_token.type == TokenType::WONDERING) {
        advance(); // skip 'wondering'
        if (current_token.type != TokenType::LPAREN)
            throw std::runtime_error("Expected ( after 'wondering'");
        advance();
        auto cond = expr();
        if (current_token.type != TokenType::RPAREN)
            throw std::runtime_error("Expected ) after condition");
        advance();

        if (current_token.type != TokenType::LBRACE)
            throw std::runtime_error("Expected { after 'wondering' condition");
        advance();

        std::vector<std::shared_ptr<Node>> body_nodes;
        while (current_token.type != TokenType::RBRACE)
            body_nodes.push_back(parse_statement()); // parse linha por linha

        advance(); // pula RBRACE
        return std::make_shared<WonderingNode>(cond, body_nodes);

    } else {
        throw std::runtime_error("Invalid Statement");
    }
}
