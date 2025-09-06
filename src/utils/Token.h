#pragma once
#include <string>

enum class TokenType {
    NUMBER,
    IDENTIFIER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    ASSIGN,
    SHOW,
    WONDERING,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    GT,
    LT,
    GE,
    LE,
    EQ,
    NE,
    END_OF_FILE,
};


struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};
