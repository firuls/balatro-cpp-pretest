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

void ScoringSystem::listModifiers() const {
    for (const auto& mod : modifiers) {
        std::cout << "- " << mod->getName() << "\n";
    }
}