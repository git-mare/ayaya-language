#include "Lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& input) : text(input), pos(0) {
    current_char = text.empty() ? '\0' : text[pos];
}

void Lexer::advance() {
    pos++;
    current_char = (pos < text.size()) ? text[pos] : '\0';
}

void Lexer::skip_whitespace() {
    while (current_char != '\0' && isspace(current_char))
        advance();
}

std::string Lexer::number() {
    std::string result;
    while (current_char != '\0' && isdigit(current_char)) {
        result += current_char;
        advance();
    }
    return result;
}

std::string Lexer::identifier() {
    std::string result;
    while (current_char != '\0' && (isalpha(current_char) || isdigit(current_char) || current_char == '_')) {
        result += current_char;
        advance();
    }
    return result;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (current_char != '\0') {
        
        if (current_char == '/' && pos + 1 < text.size() && text[pos + 1] == '/') {
            while (current_char != '\0' && current_char != '\n')
                advance();
            continue;
        }

        if (isspace(current_char)) {
            skip_whitespace();
            continue;
        }

        if (isdigit(current_char)) {
            tokens.push_back(Token(TokenType::NUMBER, number()));
            continue;
        }
        
        if (current_char == '>') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::GE, ">="));
                advance();
            } else {
                tokens.push_back(Token(TokenType::GT, ">"));
            }
            advance();
            continue;
        }

        if (current_char == '<') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::LE, "<="));
                advance();
            } else {
                tokens.push_back(Token(TokenType::LT, "<"));
            }
            advance();
            continue;
        }

        if (current_char == '=') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::EQ, "=="));
                advance();
            } else {
                tokens.push_back(Token(TokenType::ASSIGN, "="));
            }
            advance();
            continue;
        }

        if (current_char == '!') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::NE, "!="));
                advance();
            }
            advance();
            continue;
        }


        if (isalpha(current_char) || current_char == '_') {
            std::string id = identifier();
            if (id == "show")
                tokens.push_back(Token(TokenType::SHOW, id));
            else if (id == "wondering")
                tokens.push_back(Token(TokenType::WONDERING, id));
            else
                tokens.push_back(Token(TokenType::IDENTIFIER, id));
            continue;
        }

        switch (current_char) {
            case '+': tokens.push_back(Token(TokenType::PLUS, "+")); break;
            case '-': tokens.push_back(Token(TokenType::MINUS, "-")); break;
            case '*': tokens.push_back(Token(TokenType::MUL, "*")); break;
            case '/': tokens.push_back(Token(TokenType::DIV, "/")); break;
            case '=': tokens.push_back(Token(TokenType::ASSIGN, "=")); break;
            case '(': tokens.push_back(Token(TokenType::LPAREN, "(")); break;
            case ')': tokens.push_back(Token(TokenType::RPAREN, ")")); break;
            case '{': tokens.push_back(Token(TokenType::LBRACE, "{")); break;
            case '}': tokens.push_back(Token(TokenType::RBRACE, "}")); break;
            default:
                // unknown character
                break;
        }

        advance();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, ""));
    return tokens;
}
