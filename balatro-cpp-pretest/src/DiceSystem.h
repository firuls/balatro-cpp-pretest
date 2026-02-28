#pragma once
#include <vector>

class DiceSystem {
public:
    std::vector<int> rollDice(int count);
    void rerollDice(std::vector<int>& dice);
};
