#pragma once
#include <string>
#include <vector>
#include "../utils/Token.h"

class Lexer {
private:
    std::string text;
    size_t pos;
    char current_char;
    int line;
    int col;

    void advance();
    void skip_whitespace();
    std::string identifier();
    void error(const std::string& msg);

public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();
};
