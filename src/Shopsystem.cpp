#include "ShopSystem.h"
#include "modifiers/GlassDiceModifier.h"
#include "modifiers/HellDiceModifier.h"
#include "modifiers/DodecaDiceModifier.h"
#include "modifiers/HollowDiceModifier.h"
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <cstdlib>

namespace {
const std::vector<std::string> kComboNames = {
    "High Dice",
    "Doble Luck",
    "Chain",
    "Triple Mirror",
    "Doble Chain",
    "Doble God",
    "Mirror God",
    "Gembling God",
    "Golden Chain"
};

const std::map<std::string, int> kComboBaseScore = {
    {"High Dice", 4},
    {"Doble Luck", 8},
    {"Chain", 10},
    {"Triple Mirror", 30},
    {"Doble Chain", 15},
    {"Doble God", 20},
    {"Mirror God", 40},
    {"Gembling God", 45},
    {"Golden Chain", 50}
};

const int kFlatBonusPrice = 5;
const int kGlassDicePrice = 7;
const int kHellDicePrice = 8;
const int kDodecaDicePrice = 10;
const int kHollowDicePrice = 9;

std::vector<int> buildRandomShopOffers() {
    std::vector<int> allItems = {1, 2, 3, 4, 5, 6};
    for (int i = static_cast<int>(allItems.size()) - 1; i > 0; --i) {
        const int j = rand() % (i + 1);
        std::swap(allItems[i], allItems[j]);
    }
    allItems.resize(3);
    return allItems;
}

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
    const std::vector<int> offeredItems = buildRandomShopOffers();

    while (true) {
        std::cout << "\n=== SHOP ===\n";
        std::cout << "Coin: " << coins << "\n";
        for (size_t i = 0; i < offeredItems.size(); ++i) {
            const int item = offeredItems[i];
            if (item == 1) {
                std::cout << (i + 1) << ". Flat Bonus (" << kFlatBonusPrice << " coin)\n";
            } else if (item == 2) {
                std::cout << (i + 1) << ". Buff Combo (Lv. max 5)\n";
            } else if (item == 3) {
                std::cout << (i + 1) << ". Dice Kaca (Glass Dice Modifier, "
                          << kGlassDicePrice << " coin)\n";
            } else if (item == 4) {
                std::cout << (i + 1) << ". Hell Dice (" << kHellDicePrice << " coin)\n";
            } else if (item == 5) {
                std::cout << (i + 1) << ". Dodeca Dice (" << kDodecaDicePrice << " coin)\n";
            } else if (item == 6) {
                std::cout << (i + 1) << ". Hollow Dice (" << kHollowDicePrice << " coin)\n";
            }
        }
        std::cout << "0. Next Round\n";
        std::cout << "Combo Buff Aktif:\n";
        scoringSystem.listComboBuffs();

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break;
        }

        if (choice < 1 || choice > static_cast<int>(offeredItems.size())) {
            std::cout << "Pilihan tidak valid.\n";
            continue;
        }

        const int selectedItem = offeredItems[choice - 1];

        if (selectedItem == 1) {
            if (!buyWithCoins(coins, kFlatBonusPrice)) {
                std::cout << "Coin tidak cukup.\n";
                continue;
            }
            scoringSystem.addModifier(ModifierFactory::createModifier("flat"));
            std::cout << "Berhasil beli Flat Bonus. Sisa coin: " << coins << "\n";
        } else if (selectedItem == 2) {
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
                    continue;
                }

                if (!buyWithCoins(coins, price)) {
                    std::cout << "Coin tidak cukup.\n";
                    continue;
                }

                int level = scoringSystem.addOrUpgradeComboBuff(comboName);
                if (level == 0) {
                    std::cout << comboName << " sudah max level (Lv.5).\n";
                } else {
                    std::cout << comboName << " sekarang Lv." << level
                              << ". Sisa coin: " << coins << "\n";
                }
            }
        } else if (selectedItem == 3) {
            const int slotIndex =
                installDiceTypeBySlot(diceSystem, coins, kGlassDicePrice, "glass", "Glass Dice");
            if (slotIndex >= 0) {
                diceSystem.addDiceModifier(std::make_unique<GlassDiceModifier>(slotIndex));
                std::cout << "Glass berpeluang hancur. Jika tidak hancur:\n"
                          << "- Reroll pertama: 1 dadu di samping jadi angka sama.\n"
                          << "- Efek bisa ditumpuk dari 1 perubahan sampai 5 perubahan.\n"
                          << "- Bonus multi: Chain x10, Golden Chain x20.\n";
            }
        } else if (selectedItem == 4) {
            const int slotIndex =
                installDiceTypeBySlot(diceSystem, coins, kHellDicePrice, "hell", "Hell Dice");
            if (slotIndex >= 0) {
                diceSystem.addDiceModifier(std::make_unique<HellDiceModifier>(slotIndex));
                std::cout << "Hell: gabungan angka kiri + Hell + kanan jadi multiplier final.\n"
                          << "Contoh kiri 2, Hell 3, kanan 3 => multi x8.\n";
            }
        } else if (selectedItem == 5) {
            const int slotIndex =
                installDiceTypeBySlot(diceSystem, coins, kDodecaDicePrice, "dodeca", "Dodeca Dice");
            if (slotIndex >= 0) {
                diceSystem.addDiceModifier(std::make_unique<DodecaDiceModifier>(slotIndex));
                std::cout << "Dodeca membuka dadu bermata 12.\n"
                          << "Angka dodeca tidak menambah score satuan, tapi jadi multiplier final.\n";
            }
        } else if (selectedItem == 6) {
            const int slotIndex =
                installDiceTypeBySlot(diceSystem, coins, kHollowDicePrice, "hollow", "Hollow Dice");
            if (slotIndex >= 0) {
                diceSystem.addDiceModifier(std::make_unique<HollowDiceModifier>(slotIndex));
                std::cout << "Hollow: peluang dapat angka 4 naik 20%.\n"
                          << "Jika mendapat angka 4, score akhir dikali 20.\n";
            }
        }
    }
}
