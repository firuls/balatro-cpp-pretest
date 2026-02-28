#pragma once
#include <vector>
#include <memory>
#include <string>
#include "modifiers/IModifier.h"
#include "modifiers/ComboLevelModifier.h"

class ScoringSystem {
private:
    std::vector<std::unique_ptr<IModifier>> modifiers;
    std::vector<std::unique_ptr<ComboLevelModifier>> comboLevelModifiers;

public:
    void addModifier(std::unique_ptr<IModifier> mod);
    int calculateScore(int baseScore);
    int calculateScore(int baseScore, const std::vector<std::string>& comboNames);
    int addOrUpgradeComboBuff(const std::string& comboName);
    int getComboBuffLevel(const std::string& comboName) const;
    void listComboBuffs() const;
    void listModifiers() const;
};
