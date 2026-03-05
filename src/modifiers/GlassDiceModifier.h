#pragma once
#include "IDiceModifier.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>

class GlassDiceModifier : public IDiceModifier {
private:
    int slotIndex;
    bool broken = false;
    int stackedChanges = 0;
    static const int kMaxStackedChanges = 5;

    bool rollChance(int percent) const {
        return (rand() % 100) < percent;
    }

public:
    explicit GlassDiceModifier(int slotIndex) : slotIndex(slotIndex) {}

    void onDiceRerolled(std::vector<int>& dice, int rerolledIndex) override {
        if (broken || rerolledIndex != slotIndex) {
            return;
        }

        if (rollChance(10)) {
            broken = true;
            std::cout << "[Glass Dice] Dice kaca di slot " << (slotIndex + 1)
                      << " pecah! Efek nonaktif.\n";
            return;
        }

        const int value = dice[rerolledIndex];
        if (stackedChanges < kMaxStackedChanges) {
            stackedChanges++;
        }

        std::vector<int> neighborIndices;
        for (int dist = 1; dist < static_cast<int>(dice.size()); ++dist) {
            const int left = rerolledIndex - dist;
            const int right = rerolledIndex + dist;
            if (left >= 0) {
                neighborIndices.push_back(left);
            }
            if (right < static_cast<int>(dice.size())) {
                neighborIndices.push_back(right);
            }
        }

        int applied = 0;
        for (int idx : neighborIndices) {
            if (applied >= stackedChanges) {
                break;
            }
            dice[idx] = value;
            applied++;
        }

        std::cout << "[Glass Dice] Tidak hancur. Tumpukan perubahan: " << stackedChanges
                  << " aktif dari slot " << (slotIndex + 1) << ".\n";
    }

    double getFinalScoreMultiplier(const std::vector<int>& dice) const override {
        if (broken) {
            return 1.0;
        }

        bool isGoldenChain = false;
        if (dice.size() == 5) {
            std::vector<int> sorted = dice;
            std::sort(sorted.begin(), sorted.end());
            isGoldenChain = true;
            for (size_t i = 1; i < sorted.size(); ++i) {
                if (sorted[i] != sorted[i - 1] + 1) {
                    isGoldenChain = false;
                    break;
                }
            }
        }
        if (isGoldenChain) {
            return 20.0;
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

        if (hasChain3) {
            return 10.0;
        }

        return 1.0;
    }

    std::string getName() const override {
        std::string status = broken ? "(Broken)" : "(Active)";
        return "Glass Dice Slot " + std::to_string(slotIndex + 1) + " " + status;
    }
};
