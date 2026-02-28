#include "ScoringSystem.h"
#include <iostream>

void ScoringSystem::addModifier(std::unique_ptr<IModifier> mod) {
    modifiers.push_back(std::move(mod));
}

int ScoringSystem::calculateScore(int baseScore) {
    int score = baseScore;

    for (auto& mod : modifiers) {
        score = mod->apply(score);
    }

    return score;
}

int ScoringSystem::calculateScore(int baseScore, const std::vector<std::string>& comboNames) {
    int comboBuffBonus = 0;

    for (const auto& comboMod : comboLevelModifiers) {
        comboBuffBonus += comboMod->getBonusIfActive(comboNames);
    }

    return calculateScore(baseScore + comboBuffBonus);
}

int ScoringSystem::addOrUpgradeComboBuff(const std::string& comboName) {
    for (auto& comboMod : comboLevelModifiers) {
        if (comboMod->isForCombo(comboName)) {
            if (!comboMod->canLevelUp()) {
                return 0;
            }
            return comboMod->levelUp();
        }
    }

    std::unique_ptr<ComboLevelModifier> newMod =
        ComboLevelModifier::createForCombo(comboName);
    if (newMod == nullptr) {
        return 0;
    }

    comboLevelModifiers.push_back(std::move(newMod));
    return 1;
}

int ScoringSystem::getComboBuffLevel(const std::string& comboName) const {
    for (const auto& comboMod : comboLevelModifiers) {
        if (comboMod->isForCombo(comboName)) {
            return comboMod->getLevel();
        }
    }
    return 0;
}

void ScoringSystem::listComboBuffs() const {
    if (comboLevelModifiers.empty()) {
        std::cout << "- (none)\n";
        return;
    }

    for (const auto& comboMod : comboLevelModifiers) {
        std::cout << "- " << comboMod->getComboName()
                  << " Lv." << comboMod->getLevel() << "\n";
    }
}

void ScoringSystem::listModifiers() const {
    if (modifiers.empty()) {
        std::cout << "- (none)\n";
        return;
    }

    for (const auto& mod : modifiers) {
        std::cout << "- " << mod->getName() << "\n";
    }
}
