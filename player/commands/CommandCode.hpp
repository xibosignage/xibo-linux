#pragma once

#include "common/types/internal/StrongType.hpp"

#include <string>

struct CommandCode : StrongType<std::string>
{
    using StrongType::StrongType;
};

inline bool operator<(const CommandCode& first, const CommandCode& second)
{
    return static_cast<std::string>(first) < static_cast<std::string>(second);
}
