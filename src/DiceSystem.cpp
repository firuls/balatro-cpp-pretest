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
        }
    }
}
