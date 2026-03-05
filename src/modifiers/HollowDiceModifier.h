#pragma once
#include "IDiceModifier.h"
#include <cstdlib>

class HollowDiceModifier : public IDiceModifier {
private:
    int slotIndex;

public:
    explicit HollowDiceModifier(int slotIndex) : slotIndex(slotIndex) {}

    int adjustRolledValueForSlot(
        int targetSlotIndex,
        int rolledValue,
        int sides
    ) const override {
        if (targetSlotIndex != slotIndex) {
            return rolledValue;
        }

        if (rolledValue == 4) {
            return rolledValue;
        }

        if ((rand() % 100) < 20) {
            return 4;
        }

        return rolledValue;
    }

    double getFinalScoreMultiplier(const std::vector<int>& dice) const override {
        if (slotIndex < 0 || slotIndex >= static_cast<int>(dice.size())) {
            return 1.0;
        }

        if (dice[slotIndex] == 4) {
            return 20.0;
        }

        return 1.0;
    }

    std::string getName() const override {
        return "Hollow Dice Slot " + std::to_string(slotIndex + 1);
    }
};
