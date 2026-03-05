#include "ModifierFactory.h"
#include "modifiers/FlatBonusModifier.h"

std::unique_ptr<IModifier> ModifierFactory::createModifier(const std::string& type) {
    if (type == "flat")
        return std::make_unique<FlatBonusModifier>();

    return nullptr;
}
