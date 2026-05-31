// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include "tree.h"

int main() {
    std::vector<char> symbols = { '1', '2', '3' };
    PMTree tree(symbols);

    std::cout << "All permutations:\n";
    auto all = getAllPerms(tree);
    for (const auto& p : all) {
        for (char c : p) std::cout << c;
        std::cout << ' ';
    }
    std::cout << "\n\n";

    std::cout << "getPerm1(tree, 2): ";
    auto p1 = getPerm1(tree, 2);
    for (char c : p1) std::cout << c;
    std::cout << "\n";

    std::cout << "getPerm2(tree, 2): ";
    auto p2 = getPerm2(tree, 2);
    for (char c : p2) std::cout << c;
    std::cout << "\n\n";

    std::ofstream data("result/data.csv");
    data << "n,getAllPerms,getPerm1,getPerm2\n";

    std::mt19937 rng(42);
    const int trials = 20;

    for (int n = 2; n <= 10; ++n) {
        std::vector<char> abc;
        for (int i = 0; i < n; ++i) abc.push_back('A' + i);

        PMTree t(abc);
        int total = t.getTotalPerms();

        auto start = std::chrono::high_resolution_clock::now();
        volatile auto allPerms = getAllPerms(t);
        auto end = std::chrono::high_resolution_clock::now();
        double timeAll = std::chrono::duration<double>(end - start).count();

        std::uniform_int_distribution<int> dist(1, total);
        double time1 = 0.0, time2 = 0.0;
        for (int i = 0; i < trials; ++i) {
            int num = dist(rng);

            start = std::chrono::high_resolution_clock::now();
            volatile auto res1 = getPerm1(t, num);
            end = std::chrono::high_resolution_clock::now();
            time1 += std::chrono::duration<double>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            volatile auto res2 = getPerm2(t, num);
            end = std::chrono::high_resolution_clock::now();
            time2 += std::chrono::duration<double>(end - start).count();
        }
        time1 /= trials;
        time2 /= trials;

        data << n << "," << timeAll << "," << time1 << "," << time2 << "\n";
        std::cout << "n=" << n << " total=" << total
            << " all=" << timeAll << "s perm1=" << time1
            << "s perm2=" << time2 << "s\n";
    }
    data.close();
    std::cout << "Experiment data saved to result/data.csv\n";
    return 0;
}
