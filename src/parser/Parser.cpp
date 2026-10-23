#include "Parser.h"
#include <stdexcept>
#include <utility>

Parser::Parser(const std::vector<Token>& t) : tokens(t), pos(0), current_token(t[0]) {}

void Parser::advance() {
    pos++;
    if (pos < tokens.size()) current_token = tokens[pos];
}

void Parser::error(const std::string& msg) {
    throw std::runtime_error("Line " + std::to_string(current_token.line) +
                             ", Col " + std::to_string(current_token.col) + ": " + msg);
}

std::shared_ptr<Node> Parser::factor() {
    if (current_token.type == TokenType::NUMBER) {
        int value = std::stoi(current_token.value);
        advance();
        return std::make_shared<NumberNode>(value);
    } else if (current_token.type == TokenType::FLOAT) {
        double value = std::stod(current_token.value);
        advance();
        return std::make_shared<FloatNode>(value);
    } else if (current_token.type == TokenType::STRING) {
        std::string value = current_token.value;
        advance();
        return std::make_shared<StringNode>(value);
    } else if (current_token.type == TokenType::BOOL) {
        bool value = (current_token.value == "legit");
        advance();
        return std::make_shared<BoolNode>(value);
    } else if (current_token.type == TokenType::IDENTIFIER) {
        std::string name = current_token.value;
        advance();
        return std::make_shared<VarNode>(name);
    } else if (current_token.type == TokenType::LPAREN) {
        advance();
        auto node = expr();
        if (current_token.type != TokenType::RPAREN)
            error("Expected )");
        advance();
        return node;
    }
    error("Invalid Factor");
    return nullptr;
}

std::shared_ptr<Node> Parser::unary() {
    if (current_token.type == TokenType::MINUS) {
        advance();
        return std::make_shared<UnaryOpNode>("-", unary());
    }
    if (current_token.type == TokenType::NOT) {
        advance();
        return std::make_shared<UnaryOpNode>("!", unary());
    }
    return factor();
}

std::shared_ptr<Node> Parser::term() {
    auto node = unary();
    while (current_token.type == TokenType::MUL || current_token.type == TokenType::DIV) {
        std::string op = current_token.value;
        advance();
        node = std::make_shared<BinOpNode>(node, op, unary());
    }
    return node;
}

std::shared_ptr<Node> Parser::additive() {
    auto node = term();
    while (current_token.type == TokenType::PLUS || current_token.type == TokenType::MINUS) {
        std::string op = current_token.value;
        advance();
        node = std::make_shared<BinOpNode>(node, op, term());
    }
    return node;
}

std::shared_ptr<Node> Parser::comparison() {
    auto node = additive();
    while (current_token.type == TokenType::GT || current_token.type == TokenType::LT ||
           current_token.type == TokenType::GE || current_token.type == TokenType::LE ||
           current_token.type == TokenType::EQ || current_token.type == TokenType::NE) {
        std::string op = current_token.value;
        advance();
        auto right = additive();
        node = std::make_shared<BinOpNode>(node, op, right);
    }
    return node;
}

std::shared_ptr<Node> Parser::and_expr() {
    auto node = comparison();
    while (current_token.type == TokenType::AND) {
        advance();
        auto right = comparison();
        node = std::make_shared<BinOpNode>(node, "&&", right);
    }
    return node;
}

std::shared_ptr<Node> Parser::expr() {
    auto node = and_expr();
    while (current_token.type == TokenType::OR) {
        advance();
        auto right = and_expr();
        node = std::make_shared<BinOpNode>(node, "||", right);
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

std::vector<std::shared_ptr<Node>> Parser::parse_block() {
    std::vector<std::shared_ptr<Node>> body;
    while (current_token.type != TokenType::RBRACE) {
        if (current_token.type == TokenType::END_OF_FILE)
            error("Expected }");
        body.push_back(parse_statement());
    }
    advance();
    return body;
}

std::shared_ptr<Node> Parser::parse_statement() {
    // Variable declaration (ur name = value)
    if (current_token.type == TokenType::UR) {
        advance();
        if (current_token.type != TokenType::IDENTIFIER) error("Expected identifier after 'ur'");
        std::string var_name = current_token.value;
        advance();

        if (current_token.type != TokenType::ASSIGN) error("Expected =");
        advance();

        auto value = expr();
        return std::make_shared<AssignNode>(var_name, value);
    }
    // Reassignment (name = value)
    else if (current_token.type == TokenType::IDENTIFIER) {
        std::string var_name = current_token.value;
        advance();
        if (current_token.type != TokenType::ASSIGN) error("Expected =");
        advance();
        auto value = expr();
        return std::make_shared<AssignNode>(var_name, value);
    } else if (current_token.type == TokenType::SHOW) {
        advance();
        if (current_token.type != TokenType::LPAREN) error("Expected ( after show");
        advance();
        auto value = expr();
        if (current_token.type != TokenType::RPAREN) error("Expected )");
        advance();
        return std::make_shared<ShowNode>(value);
    } else if (current_token.type == TokenType::WONDERING) {
        advance();
        if (current_token.type != TokenType::LPAREN) error("Expected ( after 'wondering'");
        advance();
        auto cond = expr();
        if (current_token.type != TokenType::RPAREN) error("Expected ) after condition");
        advance();

        if (current_token.type != TokenType::LBRACE) error("Expected { after 'wondering' condition");
        advance();

        auto body = parse_block();

        std::vector<std::pair<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>>> else_ifs;
        while (current_token.type == TokenType::ACTUALLY) {
            advance();
            if (current_token.type != TokenType::LPAREN) error("Expected ( after 'actually'");
            advance();
            auto else_cond = expr();
            if (current_token.type != TokenType::RPAREN) error("Expected ) after 'actually' condition");
            advance();
            if (current_token.type != TokenType::LBRACE) error("Expected { after 'actually' condition");
            advance();
            else_ifs.push_back({else_cond, parse_block()});
        }

        std::vector<std::shared_ptr<Node>> else_body;
        if (current_token.type == TokenType::NAH) {
            advance();
            if (current_token.type != TokenType::LBRACE) error("Expected { after 'nah'");
            advance();
            else_body = parse_block();
        }

        // Build the else-if chain from the last branch backwards.
        for (int i = static_cast<int>(else_ifs.size()) - 1; i >= 0; i--) {
            else_body = { std::make_shared<WonderingNode>(else_ifs[i].first, else_ifs[i].second, else_body) };
        }

        return std::make_shared<WonderingNode>(cond, body, else_body);
    } else {
        error("Invalid Statement");
    }
    return nullptr;
}
