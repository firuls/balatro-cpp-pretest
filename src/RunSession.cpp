#include "RunSession.h"
#include "BossSystem.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <sstream>

namespace {
const std::string kColorRed = "\033[31m";
const std::string kColorGreen = "\033[32m";
const std::string kColorReset = "\033[0m";

struct HandScoreBreakdown {
    EvalResult evalResult;
    int diceValueScore = 0;
    int scoreBeforeMultiplier = 0;
    double totalMultiplier = 1.0;
    int finalScore = 0;
};

std::string formatMultiplier(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    std::string out = oss.str();
    while (!out.empty() && out.back() == '0') {
        out.pop_back();
    }
    if (!out.empty() && out.back() == '.') {
        out.pop_back();
    }
    return out;
}

std::string getTargetColor(int totalScore, int targetScore) {
    return totalScore >= targetScore ? kColorGreen : kColorRed;
}

int getRoundTargetScore(int round) {
    if (round == 1) return 300;
    if (round == 2) return 500;
    if (round == 3) return 700;
    return 900;
}

void showComboList(const ScoringSystem& scoringSystem) {
    struct ComboInfo {
        const char* name;
        int baseScore;
        double multiplier;
        const char* howToGet;
    };

    const ComboInfo combos[] = {
        {"High Dice", 4, 1, "tepat 1 dadu bernilai 5 atau 6"},
        {"Doble Luck", 8, 2, "minimal 1 pasang angka sama"},
        {"Chain", 10, 3, "ada urutan 3 angka berurutan (contoh 2-3-4)"},
        {"Triple Mirror", 30, 4, "minimal 3 dadu dengan angka sama"},
        {"Doble Chain", 15, 4, "minimal 2 pasang angka sama"},
        {"Doble God", 20, 5, "minimal 2 dadu angka 6"},
        {"Mirror God", 40, 6, "minimal 4 dadu dengan angka sama"},
        {"Gembling God", 45, 8, "minimal 4 dadu angka 6"},
        {"Golden Chain", 50, 10, "5 dadu berurutan (straight penuh)"}
    };

    std::cout << "Combo List (rarity mudah -> sulit):\n";
    for (const ComboInfo& combo : combos) {
        const int level = scoringSystem.getComboBuffLevel(combo.name);
        const double currentScore = ComboLevelModifier::scaleByLevel(combo.baseScore, level);
        const double currentMulti = ComboLevelModifier::scaleByLevel(combo.multiplier, level);
        std::cout << "- " << combo.name
                  << ": " << static_cast<int>(std::round(currentScore))
                  << ", Multi " << formatMultiplier(currentMulti)
                  << " | Cara dapat: " << combo.howToGet;
        std::cout << " | Buff Lv." << level;
        std::cout << "\n";
    }
}

bool askPlayAgain() {
    std::cout << "\nPlay again? (y/n): ";
    char answer;
    std::cin >> answer;
    return answer == 'y' || answer == 'Y';
}

void showDiceWithTypes(
    const std::vector<int>& dice,
    const DiceSystem& diceSystem,
    const std::string& comboText
) {
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
    std::cout << "\nCombo      : " << kColorGreen << comboText << kColorReset << "\n";
}

std::string joinComboNames(const std::vector<std::string>& comboNames) {
    std::string comboText;
    for (size_t i = 0; i < comboNames.size(); ++i) {
        comboText += comboNames[i];
        if (i + 1 < comboNames.size()) {
            comboText += ", ";
        }
    }
    return comboText;
}

HandScoreBreakdown evaluateHand(
    DiceEvaluator& evaluator,
    const DiceSystem& diceSystem,
    ScoringSystem& scoringSystem,
    const std::vector<int>& dice,
    int handPenalty
) {
    HandScoreBreakdown breakdown;
    breakdown.evalResult = scoringSystem.applyComboUpgrade(evaluator.evaluate(dice));
    breakdown.diceValueScore = diceSystem.calculateDiceValueScore(dice);
    const double diceFinalMultiplier = diceSystem.calculateFinalScoreMultiplier(dice);

    const int baseScore = breakdown.evalResult.baseScore + breakdown.diceValueScore;
    breakdown.scoreBeforeMultiplier =
        scoringSystem.calculateScore(baseScore, breakdown.evalResult.comboNames);
    breakdown.totalMultiplier = breakdown.evalResult.totalMultiplier * diceFinalMultiplier;
    const double rawFinalScore =
        (static_cast<double>(breakdown.scoreBeforeMultiplier) * breakdown.totalMultiplier) - handPenalty;
    breakdown.finalScore =
        std::max(0, static_cast<int>(std::round(rawFinalScore)));
    return breakdown;
}

void showRealtimeComboPreview(
    int previewScoreBeforeMultiplier,
    double previewMultiplier,
    int handPenalty
) {
    std::cout << "Preview Score (belum di multi): "
              << previewScoreBeforeMultiplier << "\n";
    std::cout << "Preview Multi: x" << formatMultiplier(previewMultiplier);
    if (handPenalty > 0) {
        std::cout << " (penalty diterapkan saat final score)";
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
                showComboList(scoringSystem);
            }

            int totalScore = 0;

            std::vector<int> dice =
                diceSystem.rollDice(diceCount);

            while(commitLeft > 0 && totalScore < targetScore) {
                const HandScoreBreakdown preview =
                    evaluateHand(evaluator, diceSystem, scoringSystem, dice, handPenalty);
                showDiceWithTypes(
                    dice,
                    diceSystem,
                    joinComboNames(preview.evalResult.comboNames)
                );
                showRealtimeComboPreview(
                    preview.scoreBeforeMultiplier,
                    preview.totalMultiplier,
                    handPenalty
                );

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
                    const HandScoreBreakdown hand =
                        evaluateHand(evaluator, diceSystem, scoringSystem, dice, handPenalty);

                    totalScore += hand.finalScore;
                    if (hand.finalScore > maxCommitScore) {
                        maxCommitScore = hand.finalScore;
                    }

                    std::cout << "Final Score: " << hand.finalScore;
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
