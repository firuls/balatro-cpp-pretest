#pragma once
#include "ScoringSystem.h"
#include "ModifierFactory.h"
#include "DiceSystem.h"

class ShopSystem {
public:
    void openShop(ScoringSystem& scoringSystem, DiceSystem& diceSystem, int& coins);
};
