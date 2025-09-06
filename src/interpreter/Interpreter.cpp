#include "Interpreter.h"
#include <iostream>
#include <stdexcept>

Interpreter::Interpreter(const std::vector<std::shared_ptr<Node>>& instr) : instructions(instr) {}

int Interpreter::eval(std::shared_ptr<Node> node) {
    if (auto n = dynamic_cast<NumberNode*>(node.get())) return n->value;
    if (auto v = dynamic_cast<VarNode*>(node.get())) {
        if (variables.find(v->name) == variables.end())
            throw std::runtime_error("Variable not defined " + v->name);
        return variables[v->name];
    }
    if (auto b = dynamic_cast<BinOpNode*>(node.get())) {
        int l = eval(b->left);
        int r = eval(b->right);
        if (b->op == "+") return l + r;
        if (b->op == "-") return l - r;
        if (b->op == "*") return l * r;
        if (b->op == "/") return l / r;
        if (b->op == ">") return l > r;
        if (b->op == "<") return l < r;
        if (b->op == ">=") return l >= r;
        if (b->op == "<=") return l <= r;
        if (b->op == "==") return l == r;
        if (b->op == "!=") return l != r;
        throw std::runtime_error("Invalid operator: " + b->op);
    }
    if (auto w = std::dynamic_pointer_cast<WonderingNode>(node)) {
        if (eval(w->condition)) {
            for (auto& n : w->body) {
                eval(n);
            }
        }
        return 0;
    }
    if (auto s = std::dynamic_pointer_cast<ShowNode>(node)) {
        std::cout << eval(s->expr) << std::endl;
        return 0;
    }
    throw std::runtime_error("Error analyzing node");
}


void Interpreter::run() {
    for (auto& instr : instructions) {
        if (auto a = std::dynamic_pointer_cast<AssignNode>(instr)) {
            variables[a->name] = eval(a->value);
        } else if (auto s = std::dynamic_pointer_cast<ShowNode>(instr)) {
            std::cout << eval(s->expr) << std::endl;
        } else if (auto w = std::dynamic_pointer_cast<WonderingNode>(instr)) {
            eval(w);
        }
    }
}