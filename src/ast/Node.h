#pragma once
#include <string>
#include <memory>
#include <map>

class Node {
public:
    virtual ~Node() = default;
};

class NumberNode : public Node {
public:
    int value;
    NumberNode(int v) : value(v) {}
};

class VarNode : public Node {
public:
    std::string name;
    VarNode(const std::string& n) : name(n) {}
};

class BinOpNode : public Node {
public:
    std::shared_ptr<Node> left;
    std::string op;
    std::shared_ptr<Node> right;

    BinOpNode(std::shared_ptr<Node> l, const std::string& o, std::shared_ptr<Node> r)
        : left(l), op(o), right(r) {}
};

class AssignNode : public Node {
public:
    std::string name;
    std::shared_ptr<Node> value;
    AssignNode(const std::string& n, std::shared_ptr<Node> v) : name(n), value(v) {}
};

class ShowNode : public Node {
public:
    std::shared_ptr<Node> expr;
    ShowNode(std::shared_ptr<Node> e) : expr(e) {}
};

class WonderingNode : public Node {
public:
    std::shared_ptr<Node> condition;
    std::vector<std::shared_ptr<Node>> body;

    WonderingNode(std::shared_ptr<Node> cond, const std::vector<std::shared_ptr<Node>>& b)
        : condition(cond), body(b) {}
};
