#pragma once

#include "common/types/internal/StrongType.hpp"

#include <string>

struct Hostname : public StrongType<std::string>
{
    using StrongType::StrongType;
};
