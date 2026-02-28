#include "RunSession.h"
#include <iostream>
#include <string>

namespace {
int getRoundTargetScore(int round) {
    if (round == 1) return 500;
    if (round == 2) return 900;
    return 1400;
}

void showComboList() {
    std::cout
        << "Combo List (rarity mudah -> sulit):\n"
        << "- Doble Luck (pair): 140\n"
        << "- Doble Chain (2 pair, contoh 11 33): 260\n"
        << "- Chain (3 angka berurutan): 220\n"
        << "- Doble God (minimal 2 angka 6, termasuk 666): 300\n"
        << "- High Dice (semua dadu hanya 5/6): 420\n"
        << "- Gembling God (minimal 4 angka 6): 600\n"
        << "- Golden Chain (5 angka berurutan): 800\n"
        << "Semua combo bisa ditumpuk jika syarat terpenuhi.\n";
}
}

void RunSession::start() {

    for(int round = 1; round <= 3; round++) {

        int targetScore = getRoundTargetScore(round);
        std::cout << "\n===== ROUND "
                  << round << " =====\n";
        std::cout << "Target score round ini: "
                  << targetScore << "\n";
        showComboList();

        int rerollLeft = 3;
        int commitLeft = 3;
        int totalScore = 0;

        std::vector<int> dice =
            diceSystem.rollDice(5);

        while(commitLeft > 0) {

            std::cout << "\nCurrent Dice: ";
            for(int d : dice)
                std::cout << d << " ";

            std::cout <<
            "\nScore Sekarang: " << totalScore
            << " / " << targetScore
            << "\nCommit Left: " << commitLeft
            << " | Reroll Left: " << rerollLeft
            << "\n";

            std::cout <<
            "1. Commit Dice\n"
            "2. Reroll Dice\n"
            "0. End Round\n";

            int choice;
            std::cin >> choice;

            if(choice == 1) {

                EvalResult result =
                    evaluator.evaluate(dice);
                int baseScore = result.baseScore;

                int finalScore =
                    scoringSystem.calculateScore(baseScore);

                totalScore += finalScore;

                std::string comboText;
                for (size_t i = 0; i < result.comboNames.size(); ++i) {
                    comboText += result.comboNames[i];
                    if (i + 1 < result.comboNames.size()) {
                        comboText += ", ";
                    }
                }

                std::cout <<
                "Combo Didapat: " << comboText << "\n"
                <<
                "Hand Score: "
                << finalScore << "\n";

                commitLeft--;

                dice = diceSystem.rollDice(5);
            }

            else if(choice == 2 && rerollLeft > 0) {

                diceSystem.rerollDice(dice);
                rerollLeft--;
            }

            else if(choice == 0)
                break;
        }

        std::cout <<
        "\nTOTAL ROUND SCORE: "
        << totalScore << "\n";

        if (totalScore < targetScore) {
            std::cout
                << "Round gagal. Butuh minimal "
                << targetScore
                << " untuk lanjut.\n";
            break;
        }

        std::cout << "Round lolos!\n";

        shopSystem.openShop(scoringSystem);
    }

    std::cout << "\n=== RUN END ===\n";
}
