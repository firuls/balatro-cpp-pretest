#include "DiceSystem.h"
#include <cstdlib>
#include <iostream>
#include <vector>

int DiceSystem::rollValueForSlot(int slotIndex) const {
    std::string typeName = getDiceTypeNameAt(slotIndex);
    if (typeName == "dodeca") {
        return rand() % 12 + 1;
    }
    return rand() % 6 + 1;
}

std::vector<int> DiceSystem::rollDice(int count) {
    std::vector<int> dice;

    for (int i = 0; i < count; i++) {
        dice.push_back(rollValueForSlot(i));
    }

    return dice;
}

void DiceSystem::rerollDice(std::vector<int>& dice) {

    std::cout << "Choose dice index to reroll (1-" << dice.size() << ", 0 stop): ";

    while(true){
        int idx;
        std::cin >> idx;

        if(idx == 0) break;

        if(idx >= 1 && idx <= static_cast<int>(dice.size())){
            dice[idx-1] = rollValueForSlot(idx - 1);

            for (auto& mod : diceModifiers) {
                mod->onDiceRerolled(dice, idx - 1);
            }
        }
    }
}

void DiceSystem::addDiceModifier(std::unique_ptr<IDiceModifier> mod) {
    diceModifiers.push_back(std::move(mod));
}

void DiceSystem::setDiceTypeName(int slotIndex, const std::string& typeName) {
    if (slotIndex >= 0 && slotIndex < static_cast<int>(diceTypeNames.size())) {
        diceTypeNames[slotIndex] = typeName;
    }
}

std::vector<std::string> DiceSystem::getDiceTypeNames(int count) const {
    std::vector<std::string> result;
    result.reserve(count);

    for (int i = 0; i < count; ++i) {
        if (i < static_cast<int>(diceTypeNames.size())) {
            result.push_back(diceTypeNames[i]);
        } else {
            result.push_back("norml");
        }
    }

    return result;
}

std::string DiceSystem::getDiceTypeNameAt(int slotIndex) const {
    if (slotIndex >= 0 && slotIndex < static_cast<int>(diceTypeNames.size())) {
        return diceTypeNames[slotIndex];
    }
    return "norml";
}

int DiceSystem::calculateDiceValueScore(const std::vector<int>& dice) const {
    std::vector<int> perSlotScore;
    perSlotScore.reserve(dice.size());

    for (int i = 0; i < static_cast<int>(dice.size()); ++i) {
        int score = dice[i];
        if (getDiceTypeNameAt(i) == "dodeca") {
            score *= 2;
        }
        perSlotScore.push_back(score);
    }

    for (int i = 0; i < static_cast<int>(dice.size()); ++i) {
        if (getDiceTypeNameAt(i) != "hell") {
            continue;
        }

        if (i - 1 >= 0) {
            perSlotScore[i - 1] *= 3;
        }
        if (i + 1 < static_cast<int>(perSlotScore.size())) {
            perSlotScore[i + 1] *= 3;
        }
    }

    int total = 0;
    for (int score : perSlotScore) {
        total += score;
    }
    return total;
}

void DiceSystem::listDiceModifiers() const {
    if (diceModifiers.empty()) {
        std::cout << "- (none)\n";
        return;
    }

    for (const auto& mod : diceModifiers) {
        std::cout << "- " << mod->getName() << "\n";
    }
}
