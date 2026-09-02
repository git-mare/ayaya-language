#pragma once
#include <vector>
#include <memory>
#include <map>
#include <variant>
#include <string>
#include "../ast/Node.h"

using AyayaValue = std::variant<int, double, bool, std::string>;

class Interpreter {
private:
    std::vector<std::shared_ptr<Node>> instructions;
    std::map<std::string, AyayaValue> variables;

    AyayaValue eval(std::shared_ptr<Node> node);

public:
    Interpreter(const std::vector<std::shared_ptr<Node>>& instr);
    void run();
};