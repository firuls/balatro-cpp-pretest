#pragma once
#include <vector>
#include <string>

class IDiceModifier {
public:
    virtual void onDiceRerolled(std::vector<int>& dice, int rerolledIndex) {}

    virtual int getRollSidesForSlot(int slotIndex, int currentSides) const {
        return currentSides;
    }

    virtual int adjustRolledValueForSlot(
        int slotIndex,
        int rolledValue,
        int sides
    ) const {
        return rolledValue;
    }

    virtual void applyScoreEffects(
        std::vector<int>& perSlotScore,
        const std::vector<int>& dice
    ) const {}

    virtual double getFinalScoreMultiplier(const std::vector<int>& dice) const {
        return 1.0;
    }

    virtual std::string getName() const = 0;
    virtual ~IDiceModifier() = default;
};
