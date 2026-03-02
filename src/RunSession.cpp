#include "RunSession.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

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
    if (round == 3) return 700;
    return 900;
}

void showComboList() {
    std::cout
        << "Combo List (rarity mudah -> sulit):\n"
        << "- High Dice: 40\n"
        << "- Doble Luck: 70\n"
        << "- Chain: 110\n"
        << "- Doble Chain: 130\n"
        << "- Doble God: 150\n"
        << "- Gembling God: 300\n"
        << "- Golden Chain: 400\n"
        << "Semua combo bisa ditumpuk jika syarat terpenuhi.\n"
        << "Nilai angka dadu juga jadi poin dasar setiap commit.\n";
}

bool askPlayAgain() {
    std::cout << "\nPlay again? (y/n): ";
    char answer;
    std::cin >> answer;
    return answer == 'y' || answer == 'Y';
}

struct BossRoundConfig {
    int diceCount = 5;
    int targetScore = -1;
    int commitLeft = 3;
    int rerollLeft = 3;
    int handPenalty = 0;
    std::string effectName;
    std::string effectDesc;
};

BossRoundConfig buildBossRoundConfig(int maxCommitScore) {
    BossRoundConfig config;
    const int effectId = rand() % 5;

    if (effectId == 0) {
        config.diceCount = 4;
        config.effectName = "Dice Shredder";
        config.effectDesc = "1 dadu dihapus. Kamu main dengan 4 dadu.";
    } else if (effectId == 1) {
        config.targetScore = std::max(300, maxCommitScore * 2);
        config.effectName = "Score Mirror";
        config.effectDesc = "Target score jadi 2x commit score terbesar yang pernah kamu dapatkan.";
    } else if (effectId == 2) {
        config.commitLeft = 2;
        config.effectName = "Commit Seal";
        config.effectDesc = "Commit dikunci jadi hanya 2 kali.";
    } else if (effectId == 3) {
        config.rerollLeft = 1;
        config.effectName = "Reroll Jam";
        config.effectDesc = "Reroll dikunci jadi hanya 1 kali.";
    } else {
        config.handPenalty = 50;
        config.effectName = "Cursed Tax";
        config.effectDesc = "Setiap commit kena penalty score -50.";
    }

    return config;
}

void showDiceWithTypes(const std::vector<int>& dice, const DiceSystem& diceSystem) {
    const int kCellWidth = 7;

    std::cout << "\nCurrent Dice: ";
    for (int d : dice) {
        std::cout << std::left << std::setw(kCellWidth) << d;
    }

    std::vector<std::string> types =
        diceSystem.getDiceTypeNames(static_cast<int>(dice.size()));
    std::cout << "\nDice Type   : ";
    for (const std::string& type : types) {
        std::cout << std::left << std::setw(kCellWidth) << type;
    }
    std::cout << "\n";
}
}

void RunSession::start() {

    bool playAgain = true;
    while (playAgain) {
        bool runLost = false;
        int coins = 0;
        int maxCommitScore = 0;

        for (int round = 1; round <= 4; round++) {
            const bool isBossRound = (round == 4);

            int targetScore = getRoundTargetScore(round);
            int rerollLeft = 3;
            int commitLeft = 3;
            int handPenalty = 0;
            int diceCount = 5;

            if (isBossRound) {
                BossRoundConfig bossConfig = buildBossRoundConfig(maxCommitScore);
                diceCount = bossConfig.diceCount;
                if (bossConfig.targetScore > 0) {
                    targetScore = bossConfig.targetScore;
                }
                rerollLeft = bossConfig.rerollLeft;
                commitLeft = bossConfig.commitLeft;
                handPenalty = bossConfig.handPenalty;

                std::cout << "\n===== BOSS ROUND =====\n";
                std::cout << "Boss Effect: " << bossConfig.effectName << "\n";
                std::cout << bossConfig.effectDesc << "\n";
            } else {
                std::cout << "\n===== ROUND "
                          << round << " =====\n";
                showComboList();
            }

            int totalScore = 0;

            std::vector<int> dice =
                diceSystem.rollDice(diceCount);

            while(commitLeft > 0 && totalScore < targetScore) {
                showDiceWithTypes(dice, diceSystem);

                std::cout <<
                "Score Sekarang: " << totalScore
                << " / " << getTargetColor(totalScore, targetScore)
                << targetScore << kColorReset
                << "\nCommit Left: " << commitLeft
                << " | Reroll Left: " << rerollLeft
                << "\n";
                if (handPenalty > 0) {
                    std::cout << "Boss Penalty Aktif: -" << handPenalty
                              << " score tiap commit\n";
                }

                std::cout <<
                "1. Commit Dice\n"
                "2. Reroll Dice\n"
                "0. End Round\n";

                int choice;
                std::cin >> choice;

                if(choice == 1) {
                    EvalResult result =
                        evaluator.evaluate(dice);
                    const int diceValueScore = diceSystem.calculateDiceValueScore(dice);
                    const int comboBaseScore = result.baseScore;
                    const int baseScore = comboBaseScore + diceValueScore;

                    const int modifiedScore =
                        scoringSystem.calculateScore(baseScore, result.comboNames);
                    const int finalScore = std::max(0, modifiedScore - handPenalty);

                    totalScore += finalScore;
                    if (finalScore > maxCommitScore) {
                        maxCommitScore = finalScore;
                    }

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
                    "Poin Nilai Dadu: "
                    << diceValueScore << "\n"
                    <<
                    "Poin Combo: "
                    << comboBaseScore << "\n"
                    <<
                    "Hand Score: "
                    << finalScore;
                    if (handPenalty > 0) {
                        std::cout << " (setelah penalty -" << handPenalty << ")";
                    }
                    std::cout << "\n";

                    if (totalScore >= targetScore) {
                        std::cout
                            << kColorGreen
                            << "Target tercapai! Round langsung selesai."
                            << kColorReset << "\n";
                    }

                    commitLeft--;

                    dice = diceSystem.rollDice(diceCount);
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
                    << (isBossRound ? "Boss mengalahkanmu. " : "Round gagal. ")
                    << "Butuh minimal "
                    << targetScore
                    << " untuk lanjut.\n";
                runLost = true;
                break;
            }

            if (isBossRound) {
                std::cout << "Boss tumbang!\n";
                continue;
            }

            std::cout << "Round lolos!\n";
            const int baseReward = 4;
            const int actionBonus = rerollLeft + commitLeft;
            const int totalReward = baseReward + actionBonus;
            coins += totalReward;

            std::cout << "Reward coin: " << baseReward
                      << " (dasar) + " << actionBonus
                      << " (sisa commit/reroll) = "
                      << totalReward << "\n";
            std::cout << "Total coin sekarang: " << coins << "\n";

            shopSystem.openShop(scoringSystem, diceSystem, coins);
        }

        if (!runLost) {
            std::cout << "\n=== YOU WIN THE RUN ===\n";
        }
        std::cout << "=== RUN END ===\n";

        playAgain = askPlayAgain();
    }
}
