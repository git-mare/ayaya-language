#pragma once
#include <vector>
#include <memory>
#include <map>
#include "../ast/Node.h"

class Interpreter {
private:
    std::vector<std::shared_ptr<Node>> instructions;
    std::map<std::string, int> variables;

    int eval(std::shared_ptr<Node> node);

public:
    Interpreter(const std::vector<std::shared_ptr<Node>>& instr);
    void run();
};
