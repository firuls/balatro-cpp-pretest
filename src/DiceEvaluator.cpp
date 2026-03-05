#include "DiceEvaluator.h"
#include <map>
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

    int highDiceCount = 0;
    for (int d : dice) {
        if (d == 5 || d == 6) {
            highDiceCount++;
        }
    }
    bool isHighDice = (highDiceCount == 1);

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
    int maxSameCount = 0;
    for (const auto& entry : counts) {
        if (entry.second >= 2) {
            pairGroups++;
        }
        if (entry.second > maxSameCount) {
            maxSameCount = entry.second;
        }
    }

    int totalScore = 0;
    double totalMultiplier = 1.0;
    std::vector<std::string> combos;

    // Combo stackable
    if (pairGroups >= 1) {
        totalScore += 8;
        totalMultiplier = std::max(totalMultiplier, 2.0);
        combos.push_back("Doble Luck");
    }
    if (pairGroups >= 2) {
        totalScore += 15;
        totalMultiplier = std::max(totalMultiplier, 4.0);
        combos.push_back("Doble Chain");
    }
    if (hasChain3) {
        totalScore += 10;
        totalMultiplier = std::max(totalMultiplier, 3.0);
        combos.push_back("Chain");
    }
    // Doble God tetap aktif walau dapat 666, 6666, dst.
    if (sixCount >= 2) {
        totalScore += 20;
        totalMultiplier = std::max(totalMultiplier, 5.0);
        combos.push_back("Doble God");
    }
    if (isHighDice) {
        totalScore += 4;
        totalMultiplier = std::max(totalMultiplier, 1.0);
        combos.push_back("High Dice");
    }
    if (maxSameCount >= 3) {
        totalScore += 30;
        totalMultiplier = std::max(totalMultiplier, 4.0);
        combos.push_back("Triple Mirror");
    }
    if (maxSameCount >= 4) {
        totalScore += 40;
        totalMultiplier = std::max(totalMultiplier, 6.0);
        combos.push_back("Mirror God");
    }
    if (sixCount >= 4) {
        totalScore += 45;
        totalMultiplier = std::max(totalMultiplier, 8.0);
        combos.push_back("Gembling God");
    }
    if (isGoldenChain) {
        totalScore += 50;
        totalMultiplier = std::max(totalMultiplier, 10.0);
        combos.push_back("Golden Chain");
    }

    if (combos.empty()) {
        return {60, 1.0, {"No Combo"}};
    }

    return {totalScore, totalMultiplier, combos};
}
