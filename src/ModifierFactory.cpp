#include "ModifierFactory.h"
#include "modifiers/DoubleScoreModifier.h"
#include "modifiers/FlatBonusModifier.h"

std::unique_ptr<IModifier> ModifierFactory::createModifier(const std::string& type) {
    if (type == "double")
        return std::make_unique<DoubleScoreModifier>();
    else if (type == "flat")
        return std::make_unique<FlatBonusModifier>();

    return nullptr;
}