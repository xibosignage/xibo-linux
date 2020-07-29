#pragma once

#include <string>

struct KeyboardKey
{
    KeyboardKey(const std::string& key, uint32_t code) : key_(key), code_(code) {}

    const std::string& key() const;
    uint32_t code() const;

private:
    std::string key_;
    uint32_t code_;
};

bool operator==(const KeyboardKey& first, const KeyboardKey& second);
bool operator!=(const KeyboardKey& first, const KeyboardKey& second);
