#pragma once

#include "ScoringSystem.h"
#include "ShopSystem.h"
#include "DiceSystem.h"
#include "DiceEvaluator.h"

class RunSession {
private:
    ScoringSystem scoringSystem;
    ShopSystem shopSystem;
    DiceSystem diceSystem;
    DiceEvaluator evaluator;

public:
    void start();
};