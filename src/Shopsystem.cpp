#include "ShopSystem.h"
#include "modifiers/GlassDiceModifier.h"
#include <iostream>
#include <memory>
#include <vector>
#include <map>

namespace {
const std::vector<std::string> kComboNames = {
    "High Dice",
    "Doble Luck",
    "Chain",
    "Doble Chain",
    "Doble God",
    "Gembling God",
    "Golden Chain"
};

const std::map<std::string, int> kComboBaseScore = {
    {"High Dice", 40},
    {"Doble Luck", 70},
    {"Chain", 110},
    {"Doble Chain", 130},
    {"Doble God", 150},
    {"Gembling God", 300},
    {"Golden Chain", 400}
};

const int kDoubleScorePrice = 9;
const int kFlatBonusPrice = 5;
const int kGlassDicePrice = 7;
const int kHellDicePrice = 8;
const int kDodecaDicePrice = 10;

int getComboBuffPrice(const std::string& comboName, int currentLevel) {
    auto it = kComboBaseScore.find(comboName);
    if (it == kComboBaseScore.end()) {
        return 9999;
    }

    const int valuePrice = it->second / 80;
    const int nextLevel = currentLevel + 1;
    return valuePrice + (nextLevel * 2);
}

bool buyWithCoins(int& coins, int price) {
    if (coins < price) {
        return false;
    }
    coins -= price;
    return true;
}

int installDiceTypeBySlot(DiceSystem& diceSystem, int& coins, int price, const std::string& typeName, const std::string& label) {
    if (!buyWithCoins(coins, price)) {
        std::cout << "Coin tidak cukup.\n";
        return -1;
    }

    std::cout << "Pilih slot " << label << " (1-5): ";
    int slot;
    std::cin >> slot;

    if (slot < 1 || slot > 5) {
        coins += price;
        std::cout << "Slot tidak valid.\n";
        return -1;
    }

    if (diceSystem.getDiceTypeNameAt(slot - 1) != "norml") {
        coins += price;
        std::cout << "Slot sudah terisi tipe lain.\n";
        return -1;
    }

    diceSystem.setDiceTypeName(slot - 1, typeName);
    std::cout << label << " terpasang di slot " << slot
              << ". Sisa coin: " << coins << "\n";
    return slot - 1;
}
}

void ShopSystem::openShop(ScoringSystem& scoringSystem, DiceSystem& diceSystem, int& coins) {
    std::cout << "\n=== SHOP ===\n";
    std::cout << "Coin: " << coins << "\n";
    std::cout << "1. Double Score (" << kDoubleScorePrice << " coin)\n";
    std::cout << "2. Flat Bonus (" << kFlatBonusPrice << " coin)\n";
    std::cout << "3. Buff Combo (Lv. max 5)\n";
    std::cout << "4. Dice Kaca (Glass Dice Modifier, "
              << kGlassDicePrice << " coin)\n";
    std::cout << "5. Hell Dice (" << kHellDicePrice << " coin)\n";
    std::cout << "6. Dodeca Dice (" << kDodecaDicePrice << " coin)\n";
    std::cout << "0. Skip\n";
    std::cout << "Combo Buff Aktif:\n";
    scoringSystem.listComboBuffs();

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        if (!buyWithCoins(coins, kDoubleScorePrice)) {
            std::cout << "Coin tidak cukup.\n";
            return;
        }
        scoringSystem.addModifier(ModifierFactory::createModifier("double"));
        std::cout << "Berhasil beli Double Score. Sisa coin: " << coins << "\n";
    } else if (choice == 2) {
        if (!buyWithCoins(coins, kFlatBonusPrice)) {
            std::cout << "Coin tidak cukup.\n";
            return;
        }
        scoringSystem.addModifier(ModifierFactory::createModifier("flat"));
        std::cout << "Berhasil beli Flat Bonus. Sisa coin: " << coins << "\n";
    } else if (choice == 3) {
        std::cout << "Pilih combo untuk di-buff:\n";
        for (size_t i = 0; i < kComboNames.size(); ++i) {
            const std::string& combo = kComboNames[i];
            const int currentLevel = scoringSystem.getComboBuffLevel(combo);
            if (currentLevel >= 5) {
                std::cout << (i + 1) << ". " << combo
                          << " (MAX)\n";
            } else {
                const int price = getComboBuffPrice(combo, currentLevel);
                std::cout << (i + 1) << ". " << combo
                          << " (Lv." << currentLevel << " -> Lv." << (currentLevel + 1)
                          << ", " << price << " coin)\n";
            }
        }
        std::cout << "0. Batal\n";

        int comboChoice;
        std::cin >> comboChoice;
        if (comboChoice >= 1 && comboChoice <= static_cast<int>(kComboNames.size())) {
            const std::string& comboName = kComboNames[comboChoice - 1];
            const int currentLevel = scoringSystem.getComboBuffLevel(comboName);
            const int price = getComboBuffPrice(comboName, currentLevel);

            if (currentLevel >= 5) {
                std::cout << comboName << " sudah max level (Lv.5).\n";
                return;
            }

            if (!buyWithCoins(coins, price)) {
                std::cout << "Coin tidak cukup.\n";
                return;
            }

            int level = scoringSystem.addOrUpgradeComboBuff(comboName);
            if (level == 0) {
                std::cout << comboName << " sudah max level (Lv.5).\n";
            } else {
                std::cout << comboName << " sekarang Lv." << level
                          << ". Sisa coin: " << coins << "\n";
            }
        }
    } else if (choice == 4) {
        const int slotIndex =
            installDiceTypeBySlot(diceSystem, coins, kGlassDicePrice, "glass", "Glass Dice");
        if (slotIndex >= 0) {
            diceSystem.addDiceModifier(std::make_unique<GlassDiceModifier>(slotIndex));
        }
    } else if (choice == 5) {
        installDiceTypeBySlot(diceSystem, coins, kHellDicePrice, "hell", "Hell Dice");
    } else if (choice == 6) {
        installDiceTypeBySlot(diceSystem, coins, kDodecaDicePrice, "dodeca", "Dodeca Dice");
    }
}
