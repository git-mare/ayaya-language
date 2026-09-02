#pragma once
#include <string>
#include <vector>
#include "../utils/Token.h"

class Lexer {
private:
    std::string text;
    size_t pos;
    char current_char;

    void advance();
    void skip_whitespace();
    std::string number();
    std::string identifier();
public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();
};
