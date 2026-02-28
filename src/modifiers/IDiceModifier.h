#pragma once
#include <vector>
#include <string>

class IDiceModifier {
public:
    virtual void onDiceRerolled(std::vector<int>& dice, int rerolledIndex) = 0;
    virtual std::string getName() const = 0;
    virtual ~IDiceModifier() = default;
};
