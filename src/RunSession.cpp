#include "RunSession.h"
#include <iostream>
#include <string>

namespace {
const std::string kColorRed = "\033[31m";
const std::string kColorGreen = "\033[32m";
const std::string kColorReset = "\033[0m";

std::string getTargetColor(int totalScore, int targetScore) {
    return totalScore >= targetScore ? kColorGreen : kColorRed;
}

int getRoundTargetScore(int round) {
    if (round == 1) return 300;
    if (round == 2) return 500;
    return 700;
}

void showComboList() {
    std::cout
        << "Combo List (rarity mudah -> sulit):\n"
        << "- High Dice: 80\n"
        << "- Doble Luck: 140\n"
        << "- Chain: 220\n"
        << "- Doble Chain: 260\n"
        << "- Doble God: 300\n"
        << "- Gembling God: 600\n"
        << "- Golden Chain: 800\n"
        << "Semua combo bisa ditumpuk jika syarat terpenuhi.\n";
}

bool askPlayAgain() {
    std::cout << "\nPlay again? (y/n): ";
    char answer;
    std::cin >> answer;
    return answer == 'y' || answer == 'Y';
}
}

void RunSession::start() {

    bool playAgain = true;
    while (playAgain) {
        bool runLost = false;

        for(int round = 1; round <= 3; round++) {

            int targetScore = getRoundTargetScore(round);
            std::cout << "\n===== ROUND "
                      << round << " =====\n";
            std::cout << "Target score round ini: "
                      << getTargetColor(0, targetScore)
                      << targetScore << kColorReset << "\n";
            showComboList();

            int rerollLeft = 3;
            int commitLeft = 3;
            int totalScore = 0;

            std::vector<int> dice =
                diceSystem.rollDice(5);

            while(commitLeft > 0 && totalScore < targetScore) {

                std::cout << "\nCurrent Dice: ";
                for(int d : dice)
                    std::cout << d << " ";

                std::cout <<
                "\nScore Sekarang: " << totalScore
                << " / " << getTargetColor(totalScore, targetScore)
                << targetScore << kColorReset
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
                    kColorGreen << "Combo: " << comboText << kColorReset << "\n"
                    <<
                    "Hand Score: "
                    << finalScore << "\n";

                    if (totalScore >= targetScore) {
                        std::cout
                            << kColorGreen
                            << "Target tercapai! Round langsung selesai."
                            << kColorReset << "\n";
                    }

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
                    << "\n=== YOU LOSE ===\n"
                    << "Round gagal. Butuh minimal "
                    << targetScore
                    << " untuk lanjut.\n";
                runLost = true;
                break;
            }

            std::cout << "Round lolos!\n";

            shopSystem.openShop(scoringSystem);
        }

        if (!runLost) {
            std::cout << "\n=== YOU WIN THE RUN ===\n";
        }
        std::cout << "=== RUN END ===\n";

        playAgain = askPlayAgain();
    }
}
