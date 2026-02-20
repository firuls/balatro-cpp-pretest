#pragma once
#include "ScoringSystem.h"
#include "ShopSystem.h"

class RunSession {
private:
    ScoringSystem scoringSystem;
    ShopSystem shopSystem;

public:
    void start();
};