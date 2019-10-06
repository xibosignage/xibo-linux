#pragma once

#include "common/types/internal/StrongType.hpp"

#include <string>

struct HardwareKey : public StrongType<std::string>
{
    using StrongType::StrongType;
};
