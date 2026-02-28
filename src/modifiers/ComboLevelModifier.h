#pragma once
#include <string>
#include <vector>
#include <memory>

class ComboLevelModifier {
private:
    std::string comboName;
    int baseComboScore;
    int level = 1;
    static const int kMaxLevel = 5;

public:
    ComboLevelModifier(const std::string& comboName, int baseComboScore)
        : comboName(comboName), baseComboScore(baseComboScore) {}

    static std::unique_ptr<ComboLevelModifier> createForCombo(const std::string& comboName) {
        if (comboName == "High Dice") return std::make_unique<ComboLevelModifier>(comboName, 40);
        if (comboName == "Doble Luck") return std::make_unique<ComboLevelModifier>(comboName, 70);
        if (comboName == "Chain") return std::make_unique<ComboLevelModifier>(comboName, 110);
        if (comboName == "Doble Chain") return std::make_unique<ComboLevelModifier>(comboName, 130);
        if (comboName == "Doble God") return std::make_unique<ComboLevelModifier>(comboName, 150);
        if (comboName == "Gembling God") return std::make_unique<ComboLevelModifier>(comboName, 300);
        if (comboName == "Golden Chain") return std::make_unique<ComboLevelModifier>(comboName, 400);
        return nullptr;
    }

    bool isForCombo(const std::string& name) const {
        return comboName == name;
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

    const std::string& getComboName() const {
        return comboName;
    }

    int getBonusIfActive(const std::vector<std::string>& activeCombos) const {
        for (const std::string& active : activeCombos) {
            if (active == comboName) {
                return (baseComboScore * level) / 5;
            }
        }
        return 0;
    }
};
