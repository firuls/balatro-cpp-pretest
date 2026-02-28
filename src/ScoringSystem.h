#pragma once
#include <vector>
#include <memory>
#include "modifiers/IModifier.h"

class ScoringSystem {
private:
    std::vector<std::unique_ptr<IModifier>> modifiers;

public:
    void addModifier(std::unique_ptr<IModifier> mod);
    int calculateScore(int baseScore);
    void listModifiers() const;
};