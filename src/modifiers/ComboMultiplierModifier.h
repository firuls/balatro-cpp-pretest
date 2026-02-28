#pragma once
#include "IModifier.h"

class ComboMultiplierModifier : public IModifier {
private:
    int level = 1;
    static const int kMaxLevel = 5;

public:
    int apply(int baseScore) override {
        if (baseScore <= 60) {
            return baseScore;
        }

        const int bonusPercent = 25 * level;
        return baseScore * (100 + bonusPercent) / 100;
    }

    std::string getName() const override {
        return "Combo Buff Type A Lv." + std::to_string(level);
    }

    bool canLevelUp() const {
        return level < kMaxLevel;
    }

    int levelUp() {
        if (canLevelUp()) {
            level++;
        }
        return level;
    }

    int getLevel() const {
        return level;
    }
};
