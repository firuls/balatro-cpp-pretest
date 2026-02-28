#pragma once
#include "IModifier.h"

class ComboFlatModifier : public IModifier {
private:
    int level = 1;
    static const int kMaxLevel = 5;

public:
    int apply(int baseScore) override {
        if (baseScore <= 60) {
            return baseScore;
        }

        return baseScore + (30 * level);
    }

    std::string getName() const override {
        return "Combo Buff Type B Lv." + std::to_string(level);
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
