// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>
#include <algorithm>

class PMTree {
 private:
    struct Node {
        char value;
        std::vector<Node*> children;
        int leaves;
        explicit Node(char v) : value(v), leaves(0) {}
    };
    Node* root;
    int totalPerms;

    Node* build(const std::vector<char>& available);
    void clear(Node* node);
    void collectPerms(Node* node, std::vector<char>& current,
        std::vector<std::vector<char>>& out) const;
    std::vector<char> getPermFast(Node* node, int num) const;

    friend std::vector<char> getPerm2(const PMTree& tree, int num);

 public:
    explicit PMTree(const std::vector<char>& alphabet);
    ~PMTree();

    std::vector<std::vector<char>> getAllPerms() const;
    Node* getRoot() const { return root; }
    int getTotalPerms() const { return totalPerms; }
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
