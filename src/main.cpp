#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "interpreter/Interpreter.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ayaya <file.aya>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    auto instructions = parser.parse();

    Interpreter interpreter(instructions);
    interpreter.run();

    return 0;
}
