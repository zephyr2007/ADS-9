// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"

PMTree::Node* PMTree::build(const std::vector<char>& available) {
    Node* node = new Node('\0');
    if (available.empty()) {
        node->leaves = 1;
        return node;
    }
    for (char c : available) {
        Node* child = new Node(c);
        std::vector<char> next;
        for (char x : available) {
            if (x != c) next.push_back(x);
        }
        Node* sub = build(next);
        child->children = sub->children;
        child->leaves = sub->leaves;
        delete sub;
        node->children.push_back(child);
    }
    node->leaves = 0;
    for (Node* ch : node->children) {
        node->leaves += ch->leaves;
    }
    return node;
}

void PMTree::clear(Node* node) {
    if (!node) return;
    for (Node* child : node->children) {
        clear(child);
    }
    delete node;
}

void PMTree::collectPerms(Node* node, std::vector<char>& current,
    std::vector<std::vector<char>>& out) const {
    if (node->children.empty()) {
        out.push_back(current);
        return;
    }
    for (Node* child : node->children) {
        current.push_back(child->value);
        collectPerms(child, current, out);
        current.pop_back();
    }
}

std::vector<char> PMTree::getPermFast(Node* node, int num) const {
    if (num < 1 || num > node->leaves) return {};
    std::vector<char> result;
    Node* cur = node;
    int k = num;
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
    for (char c : sorted) {
        Node* child = new Node(c);
        std::vector<char> next;
        for (char x : sorted) {
            if (x != c) next.push_back(x);
        }
        Node* sub = build(next);
        child->children = sub->children;
        child->leaves = sub->leaves;
        delete sub;
        root->children.push_back(child);
    }
    totalPerms = 0;
    for (Node* ch : root->children) {
        totalPerms += ch->leaves;
    }
    root->leaves = totalPerms;
}

PMTree::~PMTree() {
    clear(root);
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
    std::vector<std::vector<char>> result;
    std::vector<char> current;
    collectPerms(root, current, result);
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
    return tree.getPermFast(tree.getRoot(), num);
}
