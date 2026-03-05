#pragma once

#include <string>

struct BossRoundConfig {
    int diceCount = 5;
    int targetScore = -1;
    int commitLeft = 3;
    int rerollLeft = 3;
    int handPenalty = 0;
    std::string effectName;
    std::string effectDesc;
};

BossRoundConfig buildBossRoundConfig(int maxCommitScore);
