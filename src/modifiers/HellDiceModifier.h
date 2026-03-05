#pragma once
#include "IDiceModifier.h"

class HellDiceModifier : public IDiceModifier {
private:
    int slotIndex;

public:
    explicit HellDiceModifier(int slotIndex) : slotIndex(slotIndex) {}

    double getFinalScoreMultiplier(const std::vector<int>& dice) const override {
        if (slotIndex < 0 || slotIndex >= static_cast<int>(dice.size())) {
            return 1.0;
        }

        int total = dice[slotIndex];
        if (slotIndex - 1 >= 0) {
            total += dice[slotIndex - 1];
        }
        if (slotIndex + 1 < static_cast<int>(dice.size())) {
            total += dice[slotIndex + 1];
        }

        return total;
    }

    std::string getName() const override {
        return "Hell Dice Slot " + std::to_string(slotIndex + 1);
    }
};
