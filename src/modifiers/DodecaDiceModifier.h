#pragma once
#include "IDiceModifier.h"

class DodecaDiceModifier : public IDiceModifier {
private:
    int slotIndex;

public:
    explicit DodecaDiceModifier(int slotIndex) : slotIndex(slotIndex) {}

    int getRollSidesForSlot(int targetSlot, int currentSides) const override {
        if (targetSlot == slotIndex) {
            return 12;
        }
        return currentSides;
    }

    void applyScoreEffects(
        std::vector<int>& perSlotScore,
        const std::vector<int>& dice
    ) const override {
        if (slotIndex < 0 || slotIndex >= static_cast<int>(dice.size())) {
            return;
        }
        // Dodeca value is converted into multiplier, not added as unit score.
        perSlotScore[slotIndex] = 0;
    }

    double getFinalScoreMultiplier(const std::vector<int>& dice) const override {
        if (slotIndex < 0 || slotIndex >= static_cast<int>(dice.size())) {
            return 1.0;
        }
        return static_cast<double>(dice[slotIndex]);
    }

    std::string getName() const override {
        return "Dodeca Dice Slot " + std::to_string(slotIndex + 1);
    }
};
