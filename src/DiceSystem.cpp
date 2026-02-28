#include "DiceSystem.h"
#include <cstdlib>
#include <iostream>
#include <vector>

std::vector<int> DiceSystem::rollDice(int count) {
    std::vector<int> dice;

    for(int i=0;i<count;i++)
        dice.push_back(rand()%6+1);

    return dice;
}

void DiceSystem::rerollDice(std::vector<int>& dice) {

    std::cout << "Choose dice index to reroll (1-5, 0 stop): ";

    while(true){
        int idx;
        std::cin >> idx;

        if(idx == 0) break;

        if(idx >= 1 && idx <= static_cast<int>(dice.size())){
            dice[idx-1] = rand()%6+1;

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

void DiceSystem::listDiceModifiers() const {
    if (diceModifiers.empty()) {
        std::cout << "- (none)\n";
        return;
    }

    for (const auto& mod : diceModifiers) {
        std::cout << "- " << mod->getName() << "\n";
    }
}
