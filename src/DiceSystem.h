#pragma once
#include <vector>
#include <memory>
#include <string>
#include "modifiers/IDiceModifier.h"

class DiceSystem {
private:
    std::vector<std::unique_ptr<IDiceModifier>> diceModifiers;
    std::vector<std::string> diceTypeNames = {"norml", "norml", "norml", "norml", "norml"};
    int rollValueForSlot(int slotIndex) const;

public:
    std::vector<int> rollDice(int count);
    void rerollDice(std::vector<int>& dice);
    void addDiceModifier(std::unique_ptr<IDiceModifier> mod);
    void setDiceTypeName(int slotIndex, const std::string& typeName);
    std::vector<std::string> getDiceTypeNames(int count) const;
    std::string getDiceTypeNameAt(int slotIndex) const;
    int calculateDiceValueScore(const std::vector<int>& dice) const;
    double calculateFinalScoreMultiplier(const std::vector<int>& dice) const;
    void listDiceModifiers() const;
};
