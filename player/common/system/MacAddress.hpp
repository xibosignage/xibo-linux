#pragma once

#include "common/types/internal/StrongType.hpp"

#include <string>

struct MacAddress : public StrongType<std::string>
{
    using StrongType::StrongType;
};
