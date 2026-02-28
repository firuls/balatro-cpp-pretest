#pragma once
#include <vector>
#include <string>

struct EvalResult {
    int baseScore;
    std::vector<std::string> comboNames;
};

class DiceEvaluator {
public:
    EvalResult evaluate(const std::vector<int>& dice);
};
