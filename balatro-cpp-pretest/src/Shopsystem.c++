#include "ShopSystem.h"
#include <iostream>

void ShopSystem::openShop(ScoringSystem& scoringSystem) {
    std::cout << "\n=== SHOP ===\n";
    std::cout << "1. Double Score\n";
    std::cout << "2. Flat Bonus\n";
    std::cout << "0. Skip\n";

    int choice;
    std::cin >> choice;

    if (choice == 1)
        scoringSystem.addModifier(ModifierFactory::createModifier("double"));
    else if (choice == 2)
        scoringSystem.addModifier(ModifierFactory::createModifier("flat"));
}