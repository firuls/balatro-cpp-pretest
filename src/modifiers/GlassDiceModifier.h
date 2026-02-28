#pragma once
#include "IDiceModifier.h"
#include <cstdlib>
#include <iostream>

class GlassDiceModifier : public IDiceModifier {
private:
    int slotIndex;
    bool broken = false;

    bool rollChance(int percent) const {
        return (rand() % 100) < percent;
    }

public:
    explicit GlassDiceModifier(int slotIndex) : slotIndex(slotIndex) {}

    void onDiceRerolled(std::vector<int>& dice, int rerolledIndex) override {
        if (broken || rerolledIndex != slotIndex) {
            return;
        }

        if (rollChance(10)) {
            broken = true;
            std::cout << "[Glass Dice] Dice kaca di slot " << (slotIndex + 1)
                      << " pecah! Efek nonaktif.\n";
            return;
        }

        if (!rollChance(60)) {
            return;
        }

        const int value = dice[rerolledIndex];

        int left = rerolledIndex - 1;
        while (left >= 0 && rollChance(70)) {
            dice[left] = value;
            left--;
        }

        int right = rerolledIndex + 1;
        while (right < static_cast<int>(dice.size()) && rollChance(70)) {
            dice[right] = value;
            right++;
        }

        std::cout << "[Glass Dice] Resonansi aktif, angka menyebar dari slot "
                  << (slotIndex + 1) << ".\n";
    }

    std::string getName() const override {
        std::string status = broken ? "(Broken)" : "(Active)";
        return "Glass Dice Slot " + std::to_string(slotIndex + 1) + " " + status;
    }
};
