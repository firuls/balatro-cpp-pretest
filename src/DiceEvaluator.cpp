#include "DiceEvaluator.h"
#include <map>
#include <iostream>
#include <algorithm>
#include <set>

EvalResult DiceEvaluator::evaluate(const std::vector<int>& dice) {

    std::map<int, int> counts;
    for (int d : dice) {
        counts[d]++;
    }

    int sixCount = counts[6];

    std::vector<int> sorted = dice;
    std::sort(sorted.begin(), sorted.end());

    bool isGoldenChain = false;
    if (sorted.size() == 5) {
        isGoldenChain = true;
        for (size_t i = 1; i < sorted.size(); ++i) {
            if (sorted[i] != sorted[i - 1] + 1) {
                isGoldenChain = false;
                break;
            }
        }
    }

    bool isHighDice = true;
    for (int d : dice) {
        if (d != 5 && d != 6) {
            isHighDice = false;
            break;
        }
    }

    std::set<int> uniqueValues(dice.begin(), dice.end());
    std::vector<int> uniqueSorted(uniqueValues.begin(), uniqueValues.end());
    bool hasChain3 = false;
    if (uniqueSorted.size() >= 3) {
        for (size_t i = 0; i + 2 < uniqueSorted.size(); ++i) {
            if (uniqueSorted[i + 1] == uniqueSorted[i] + 1 &&
                uniqueSorted[i + 2] == uniqueSorted[i] + 2) {
                hasChain3 = true;
                break;
            }
        }
    }

    int pairGroups = 0;
    for (const auto& entry : counts) {
        if (entry.second >= 2) {
            pairGroups++;
        }
    }

    int totalScore = 0;
    std::vector<std::string> combos;

    // Combo stackable
    if (pairGroups >= 1) {
        totalScore += 140;
        combos.push_back("Doble Luck");
    }
    if (pairGroups >= 2) {
        totalScore += 260;
        combos.push_back("Doble Chain");
    }
    if (hasChain3) {
        totalScore += 220;
        combos.push_back("Chain");
    }
    // Doble God tetap aktif walau dapat 666, 6666, dst.
    if (sixCount >= 2) {
        totalScore += 300;
        combos.push_back("Doble God");
    }
    if (isHighDice) {
        totalScore += 420;
        combos.push_back("High Dice");
    }
    if (sixCount >= 4) {
        totalScore += 600;
        combos.push_back("Gembling God");
    }
    if (isGoldenChain) {
        totalScore += 800;
        combos.push_back("Golden Chain");
    }

    if (combos.empty()) {
        std::cout << "No combo\n";
        return {60, {"No Combo"}};
    }

    std::cout << "Combo stack: ";
    for (size_t i = 0; i < combos.size(); ++i) {
        std::cout << combos[i];
        if (i + 1 < combos.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "\n";

    return {totalScore, combos};
}
