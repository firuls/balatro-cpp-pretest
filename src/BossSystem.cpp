#include "BossSystem.h"

#include <algorithm>
#include <cstdlib>

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
