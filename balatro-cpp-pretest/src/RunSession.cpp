#include "RunSession.h"
#include <iostream>
#include <cstdlib>

void RunSession::start() {
    for (int round = 1; round <= 3; round++) {
        std::cout << "\n=== ROUND " << round << " ===\n";

        int baseScore = rand() % 100 + 50;
        std::cout << "Base Score: " << baseScore << "\n";

        int finalScore = scoringSystem.calculateScore(baseScore);

        std::cout << "Final Score: " << finalScore << "\n";

        std::cout << "Active Modifiers:\n";
        scoringSystem.listModifiers();

        shopSystem.openShop(scoringSystem);
    }

    std::cout << "\n=== RUN END ===\n";
}