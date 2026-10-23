#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input) : text(input), pos(0), line(1), col(1) {
    current_char = text.empty() ? '\0' : text[pos];
}

void Lexer::advance() {
    if (current_char == '\n') {
        line++;
        col = 1;
    } else {
        col++;
    }
    pos++;
    current_char = (pos < text.size()) ? text[pos] : '\0';
}

void Lexer::error(const std::string& msg) {
    throw std::runtime_error("Line " + std::to_string(line) + ", Col " + std::to_string(col) + ": " + msg);
}

void Lexer::skip_whitespace() {
    while (current_char != '\0' && isspace(current_char))
        advance();
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
        int start_line = line;
        int start_col = col;

        if (current_char == '/' && pos + 1 < text.size() && text[pos + 1] == '/') {
            while (current_char != '\0' && current_char != '\n') advance();
            continue;
        }

        if (isspace(current_char)) {
            skip_whitespace();
            continue;
        }

        // Strings
        if (current_char == '"') {
            advance();
            std::string str;
            while (current_char != '\0' && current_char != '"') {
                str += current_char;
                advance();
            }
            if (current_char == '\0') error("Unterminated string");
            advance();
            tokens.push_back(Token(TokenType::STRING, str, start_line, start_col));
            continue;
        }

        // Numbers (integers and decimals)
        if (isdigit(current_char)) {
            std::string num;
            bool has_dot = false;
            while (current_char != '\0' && (isdigit(current_char) || current_char == '.')) {
                if (current_char == '.') {
                    if (has_dot) break;
                    has_dot = true;
                }
                num += current_char;
                advance();
            }
            if (has_dot) tokens.push_back(Token(TokenType::FLOAT, num, start_line, start_col));
            else tokens.push_back(Token(TokenType::NUMBER, num, start_line, start_col));
            continue;
        }

        if (current_char == '>') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::GE, ">=", start_line, start_col));
                advance();
            } else {
                tokens.push_back(Token(TokenType::GT, ">", start_line, start_col));
            }
            advance();
            continue;
        }

        if (current_char == '<') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::LE, "<=", start_line, start_col));
                advance();
            } else {
                tokens.push_back(Token(TokenType::LT, "<", start_line, start_col));
            }
            advance();
            continue;
        }

        if (current_char == '=') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::EQ, "==", start_line, start_col));
                advance();
            } else {
                tokens.push_back(Token(TokenType::ASSIGN, "=", start_line, start_col));
            }
            advance();
            continue;
        }

        if (current_char == '!') {
            if (pos + 1 < text.size() && text[pos + 1] == '=') {
                tokens.push_back(Token(TokenType::NE, "!=", start_line, start_col));
                advance();
                advance();
            } else {
                tokens.push_back(Token(TokenType::NOT, "!", start_line, start_col));
                advance();
            }
            continue;
        }

        if (current_char == '&') {
            if (pos + 1 < text.size() && text[pos + 1] == '&') {
                tokens.push_back(Token(TokenType::AND, "&&", start_line, start_col));
                advance();
                advance();
            } else {
                error("Unexpected '&'. Did you mean '&&'?");
            }
            continue;
        }

        if (current_char == '|') {
            if (pos + 1 < text.size() && text[pos + 1] == '|') {
                tokens.push_back(Token(TokenType::OR, "||", start_line, start_col));
                advance();
                advance();
            } else {
                error("Unexpected '|'. Did you mean '||'?");
            }
            continue;
        }

        if (isalpha(current_char) || current_char == '_') {
            std::string id = identifier();
            TokenType type;
            if (id == "show") type = TokenType::SHOW;
            else if (id == "wondering") type = TokenType::WONDERING;
            else if (id == "ur") type = TokenType::UR;
            else if (id == "nah") type = TokenType::NAH;
            else if (id == "actually") type = TokenType::ACTUALLY;
            else if (id == "legit" || id == "sus") type = TokenType::BOOL;
            else type = TokenType::IDENTIFIER;
            tokens.push_back(Token(type, id, start_line, start_col));
            continue;
        }

        switch (current_char) {
            case '+': tokens.push_back(Token(TokenType::PLUS, "+", start_line, start_col)); break;
            case '-': tokens.push_back(Token(TokenType::MINUS, "-", start_line, start_col)); break;
            case '*': tokens.push_back(Token(TokenType::MUL, "*", start_line, start_col)); break;
            case '/': tokens.push_back(Token(TokenType::DIV, "/", start_line, start_col)); break;
            case '(': tokens.push_back(Token(TokenType::LPAREN, "(", start_line, start_col)); break;
            case ')': tokens.push_back(Token(TokenType::RPAREN, ")", start_line, start_col)); break;
            case '{': tokens.push_back(Token(TokenType::LBRACE, "{", start_line, start_col)); break;
            case '}': tokens.push_back(Token(TokenType::RBRACE, "}", start_line, start_col)); break;
            default:
                error(std::string("Unexpected character '") + current_char + "'");
        }

        advance();
    }

    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, col));
    return tokens;
}
