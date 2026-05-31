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

    Node* construct(const std::vector<char>& available);
    void cleanup(Node* node);
    void gatherPerms(Node* node, std::vector<char>& current,
        std::vector<std::vector<char>>& out) const;
    std::vector<char> fetchPermQuick(Node* node, int idx) const;

    friend std::vector<char> fetchPerm2(const PMTree& tree, int idx);

 public:
    explicit PMTree(const std::vector<char>& alphabet);
    ~PMTree();

    std::vector<std::vector<char>> fetchAllPerms() const;
    Node* getRoot() const { return root; }
    int getTotalPerms() const { return totalPerms; }
};

std::vector<std::vector<char>> fetchAllPerms(const PMTree& tree);
std::vector<char> fetchPerm1(const PMTree& tree, int idx);
std::vector<char> fetchPerm2(const PMTree& tree, int idx);

#endif  // INCLUDE_TREE_H_
