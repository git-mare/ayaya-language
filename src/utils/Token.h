#pragma once
#include <string>

enum class TokenType {
    NUMBER,
    FLOAT,
    STRING,
    BOOL,
    IDENTIFIER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    ASSIGN,
    SHOW,
    WONDERING,
    UR,
    NAH,
    ACTUALLY,
    AND,
    OR,
    NOT,
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
    int line;
    int col;

    Token(TokenType t, const std::string& v, int l = 0, int c = 0)
        : type(t), value(v), line(l), col(c) {}
};
