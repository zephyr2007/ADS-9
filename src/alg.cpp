// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"

PMTree::Node* PMTree::constructTree(const std::vector<char>& available) {
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
        Node* subtree = constructTree(remaining);
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

void PMTree::destroyTree(Node* node) {
    if (!node) return;
    for (Node* child : node->children) {
        destroyTree(child);
    }
    delete node;
}

void PMTree::gatherAllPerms(Node* node, std::vector<char>& current,
    std::vector<std::vector<char>>& out) const {
    if (node->children.empty()) {
        out.push_back(current);
        return;
    }
    for (Node* child : node->children) {
        current.push_back(child->value);
        gatherAllPerms(child, current, out);
        current.pop_back();
    }
}

std::vector<char> PMTree::getPermByIndex(Node* node, int idx) const {
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
        Node* subtree = constructTree(remaining);
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
    destroyTree(root);
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    gatherAllPerms(root, current, result);
    return result;
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    return tree.getAllPerms();
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    auto all = tree.getAllPerms();
    if (num < 1 || num > static_cast<int>(all.size())) {
        return {};
    }
    return all[num - 1];
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num < 1 || num > tree.getTotalPerms()) {
        return {};
    }
    return tree.getPermByIndex(tree.getRoot(), num);
}
