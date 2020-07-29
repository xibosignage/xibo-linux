#include "KeyboardKey.hpp"

const std::string& KeyboardKey::key() const
{
    return key_;
}

uint32_t KeyboardKey::code() const
{
    return code_;
}

bool operator==(const KeyboardKey& first, const KeyboardKey& second)
{
    return first.key() == second.key() && first.code() == second.code();
}

bool operator!=(const KeyboardKey& first, const KeyboardKey& second)
{
    return !(first == second);
}
