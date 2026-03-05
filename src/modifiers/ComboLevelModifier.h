#pragma once
#include <string>
#include <vector>
#include <memory>

class ComboLevelModifier {
private:
    std::string comboName;
    double baseComboScore;
    double baseComboMultiplier;
    int level = 1;
    static const int kMaxLevel = 5;

public:
    ComboLevelModifier(const std::string& comboName, double baseComboScore, double baseComboMultiplier)
        : comboName(comboName), baseComboScore(baseComboScore), baseComboMultiplier(baseComboMultiplier) {}

    static bool getBaseComboStats(const std::string& comboName, double& outScore, double& outMultiplier) {
        if (comboName == "High Dice") { outScore = 4.0; outMultiplier = 1.0; return true; }
        if (comboName == "Doble Luck") { outScore = 8.0; outMultiplier = 2.0; return true; }
        if (comboName == "Chain") { outScore = 10.0; outMultiplier = 3.0; return true; }
        if (comboName == "Triple Mirror") { outScore = 30.0; outMultiplier = 4.0; return true; }
        if (comboName == "Doble Chain") { outScore = 15.0; outMultiplier = 4.0; return true; }
        if (comboName == "Doble God") { outScore = 20.0; outMultiplier = 5.0; return true; }
        if (comboName == "Mirror God") { outScore = 40.0; outMultiplier = 6.0; return true; }
        if (comboName == "Gembling God") { outScore = 45.0; outMultiplier = 8.0; return true; }
        if (comboName == "Golden Chain") { outScore = 50.0; outMultiplier = 10.0; return true; }
        return false;
    }

    static double scaleByLevel(double baseValue, int level) {
        double value = baseValue;
        for (int i = 0; i < level; ++i) {
            value *= 1.5;
        }
        return value;
    }

    static std::unique_ptr<ComboLevelModifier> createForCombo(const std::string& comboName) {
        double baseScore = 0.0;
        double baseMultiplier = 1.0;
        if (!getBaseComboStats(comboName, baseScore, baseMultiplier)) {
            return nullptr;
        }
        return std::make_unique<ComboLevelModifier>(comboName, baseScore, baseMultiplier);
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
                return static_cast<int>(scaleByLevel(baseComboScore, level) - baseComboScore);
            }
        }
        return 0;
    }
};
