// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"

PMTree::Node* PMTree::construct(const std::vector<char>& available) {
    Node* node = new Node('\0');
    if (available.empty()) {
        node->leaves = 1;
        return node;
    }
    for (char ch : available) {
        Node* child = new Node(ch);
        std::vector<char> remaining;
        for (char x : available) {
            if (x != ch) remaining.push_back(x);
        }
        Node* subtree = construct(remaining);
        child->children = subtree->children;
        child->leaves = subtree->leaves;
        delete subtree;
        node->children.push_back(child);
    }
    node->leaves = 0;
    for (Node* ch : node->children) {
        node->leaves += ch->leaves;
    }
    return node;
}

void PMTree::cleanup(Node* node) {
    if (!node) return;
    for (Node* child : node->children) {
        cleanup(child);
    }
    delete node;
}

void PMTree::gatherPerms(Node* node, std::vector<char>& current,
    std::vector<std::vector<char>>& out) const {
    if (node->children.empty()) {
        out.push_back(current);
        return;
    }
    for (Node* child : node->children) {
        current.push_back(child->value);
        gatherPerms(child, current, out);
        current.pop_back();
    }
}

std::vector<char> PMTree::fetchPermQuick(Node* node, int idx) const {
    if (idx < 1 || idx > node->leaves) return {};
    std::vector<char> result;
    Node* cur = node;
    int k = idx;
    while (!cur->children.empty()) {
        int sum = 0;
        for (Node* child : cur->children) {
            int cnt = child->leaves;
            if (sum + cnt >= k) {
                result.push_back(child->value);
                cur = child;
                k -= sum;
                break;
            }
            sum += cnt;
        }
    }
    return result;
}

PMTree::PMTree(const std::vector<char>& alphabet) {
    std::vector<char> sorted = alphabet;
    std::sort(sorted.begin(), sorted.end());
    root = new Node('\0');
    for (char ch : sorted) {
        Node* child = new Node(ch);
        std::vector<char> remaining;
        for (char x : sorted) {
            if (x != ch) remaining.push_back(x);
        }
        Node* subtree = construct(remaining);
        child->children = subtree->children;
        child->leaves = subtree->leaves;
        delete subtree;
        root->children.push_back(child);
    }
    totalPerms = 0;
    for (Node* ch : root->children) {
        totalPerms += ch->leaves;
    }
    root->leaves = totalPerms;
}

PMTree::~PMTree() {
    cleanup(root);
}

std::vector<std::vector<char>> PMTree::fetchAllPerms() const {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    gatherPerms(root, current, result);
    return result;
}

std::vector<std::vector<char>> fetchAllPerms(const PMTree& tree) {
    return tree.fetchAllPerms();
}

std::vector<char> fetchPerm1(const PMTree& tree, int idx) {
    auto all = tree.fetchAllPerms();
    if (idx < 1 || idx > static_cast<int>(all.size())) {
        return {};
    }
    return all[idx - 1];
}

std::vector<char> fetchPerm2(const PMTree& tree, int idx) {
    if (idx < 1 || idx > tree.getTotalPerms()) {
        return {};
    }
    return tree.fetchPermQuick(tree.getRoot(), idx);
}
