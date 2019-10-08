#pragma once

#include "common/crypto/Md5Hash.hpp"
#include "common/types/internal/StrongType.hpp"

#include <string>

struct HardwareKey : public StrongType<Md5Hash>
{
    using StrongType::StrongType;
};
