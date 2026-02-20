#pragma once
#include <string>

class IModifier {
public:
    virtual int apply(int baseScore) = 0;
    virtual std::string getName() const = 0;
    virtual ~IModifier() = default;
};