#include "Interpreter.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

Interpreter::Interpreter(const std::vector<std::shared_ptr<Node>>& instr) : instructions(instr) {}

// Helpers for dynamic type coercion
double get_num(const AyayaValue& v) {
    if (auto p = std::get_if<int>(&v)) return *p;
    if (auto p = std::get_if<double>(&v)) return *p;
    if (auto p = std::get_if<bool>(&v)) return *p ? 1.0 : 0.0;
    throw std::runtime_error("Type error: expected a number for math operations");
}

// Improved helper to clean up unnecessary zeros from decimals
std::string get_string(const AyayaValue& v) {
    if (auto p = std::get_if<std::string>(&v)) return *p;
    if (auto p = std::get_if<int>(&v)) return std::to_string(*p);
    if (auto p = std::get_if<double>(&v)) {
        std::string str = std::to_string(*p);
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') str.pop_back(); // Remove the dot if it is an integer, e.g., "15." -> "15"
        return str;
    }
    if (auto p = std::get_if<bool>(&v)) return *p ? "true" : "false";
    return "";
}

// Evaluates truthiness
bool is_truthy(const AyayaValue& v) {
    if (auto p = std::get_if<bool>(&v)) return *p;
    if (auto p = std::get_if<int>(&v)) return *p != 0;
    if (auto p = std::get_if<double>(&v)) return *p != 0.0;
    if (auto p = std::get_if<std::string>(&v)) return !p->empty();
    return false;
}

AyayaValue Interpreter::eval(std::shared_ptr<Node> node) {
    // Primitive data types
    if (auto n = dynamic_cast<NumberNode*>(node.get())) return n->value;
    if (auto f = dynamic_cast<FloatNode*>(node.get())) return f->value;
    if (auto s = dynamic_cast<StringNode*>(node.get())) return s->value;
    if (auto b = dynamic_cast<BoolNode*>(node.get())) return b->value;

    // Variable resolution
    if (auto v = dynamic_cast<VarNode*>(node.get())) {
        if (variables.find(v->name) == variables.end())
            throw std::runtime_error("Variable not defined: " + v->name);
        return variables[v->name];
    }
    
    // Variable assignment (handles assignments inside blocks)
    if (auto a = dynamic_cast<AssignNode*>(node.get())) {
        AyayaValue val = eval(a->value);
        variables[a->name] = val;
        return val; 
    }
    
    // Binary Operations
    if (auto b = dynamic_cast<BinOpNode*>(node.get())) {
        AyayaValue l = eval(b->left);
        AyayaValue r = eval(b->right);

        // String Concatenation
        if (b->op == "+") {
            if (std::holds_alternative<std::string>(l) || std::holds_alternative<std::string>(r)) {
                return get_string(l) + get_string(r);
            }
        }
        
        // Numeric vs String Equality
        if (b->op == "==") {
            if (std::holds_alternative<std::string>(l) || std::holds_alternative<std::string>(r))
                return get_string(l) == get_string(r);
            return get_num(l) == get_num(r);
        }
        if (b->op == "!=") {
            if (std::holds_alternative<std::string>(l) || std::holds_alternative<std::string>(r))
                return get_string(l) != get_string(r);
            return get_num(l) != get_num(r);
        }

        // Check if both are integers to keep the primitive type intact
        bool both_ints = std::holds_alternative<int>(l) && std::holds_alternative<int>(r);

        if (both_ints) {
            int num_l = std::get<int>(l);
            int num_r = std::get<int>(r);
            if (b->op == "+") return num_l + num_r;
            if (b->op == "-") return num_l - num_r;
            if (b->op == "*") return num_l * num_r;
            if (b->op == "/") {
                if (num_r == 0) throw std::runtime_error("Division by zero");
                return num_l / num_r; 
            }
            if (b->op == ">") return num_l > num_r;
            if (b->op == "<") return num_l < num_r;
            if (b->op == ">=") return num_l >= num_r;
            if (b->op == "<=") return num_l <= num_r;
        } else {
            // If one of them is a double, bool, or forced coercion, treat everything as double
            double num_l = get_num(l);
            double num_r = get_num(r);
            if (b->op == "+") return num_l + num_r;
            if (b->op == "-") return num_l - num_r;
            if (b->op == "*") return num_l * num_r;
            if (b->op == "/") {
                if (num_r == 0.0) throw std::runtime_error("Division by zero");
                return num_l / num_r;
            }
            if (b->op == ">") return num_l > num_r;
            if (b->op == "<") return num_l < num_r;
            if (b->op == ">=") return num_l >= num_r;
            if (b->op == "<=") return num_l <= num_r;
        }
        throw std::runtime_error("Invalid operator: " + b->op);
    }
    
    // Conditionals (Wondering)
    if (auto w = std::dynamic_pointer_cast<WonderingNode>(node)) {
        if (is_truthy(eval(w->condition))) {
            for (auto& n : w->body) {
                eval(n);
            }
        }
        return 0; // Default return for statement (dummy value)
    }
    
    // Output (Show)
    if (auto s = std::dynamic_pointer_cast<ShowNode>(node)) {
        AyayaValue val = eval(s->expr);
        std::cout << get_string(val) << std::endl;
        return 0; // Default return for statement (dummy value)
    }
    
    throw std::runtime_error("Error analyzing node");
}

void Interpreter::run() {
    for (auto& instr : instructions) {
        if (auto a = std::dynamic_pointer_cast<AssignNode>(instr)) {
            variables[a->name] = eval(a->value);
        } else if (auto s = std::dynamic_pointer_cast<ShowNode>(instr)) {
            AyayaValue val = eval(s->expr);
            std::cout << get_string(val) << std::endl;
        } else if (auto w = std::dynamic_pointer_cast<WonderingNode>(instr)) {
            eval(w);
        }
    }
}