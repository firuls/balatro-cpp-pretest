#pragma once
#include <memory>
#include <string>
#include "modifiers/IModifier.h"

class ModifierFactory {
public:
    static std::unique_ptr<IModifier> createModifier(const std::string& type);
};