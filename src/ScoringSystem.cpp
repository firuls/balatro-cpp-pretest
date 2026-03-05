#include "ScoringSystem.h"
#include <iostream>
#include <cmath>

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
    (void)comboNames;
    return calculateScore(baseScore);
}

EvalResult ScoringSystem::applyComboUpgrade(const EvalResult& evalResult) const {
    if (evalResult.comboNames.size() == 1 && evalResult.comboNames[0] == "No Combo") {
        return evalResult;
    }

    double upgradedScore = 0.0;
    double upgradedMultiplier = 1.0;

    for (const std::string& comboName : evalResult.comboNames) {
        double baseScore = 0.0;
        double baseMulti = 1.0;
        if (!ComboLevelModifier::getBaseComboStats(comboName, baseScore, baseMulti)) {
            continue;
        }

        const int level = getComboBuffLevel(comboName);
        const double adjustedScore = ComboLevelModifier::scaleByLevel(baseScore, level);
        const double adjustedMulti = ComboLevelModifier::scaleByLevel(baseMulti, level);

        upgradedScore += adjustedScore;
        if (adjustedMulti > upgradedMultiplier) {
            upgradedMultiplier = adjustedMulti;
        }
    }

    EvalResult upgraded = evalResult;
    upgraded.baseScore = static_cast<int>(std::round(upgradedScore));
    upgraded.totalMultiplier = upgradedMultiplier;
    return upgraded;
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
