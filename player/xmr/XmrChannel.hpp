#pragma once

#include "common/types/internal/StrongType.hpp"

#include <string>

struct XmrChannel : StrongType<std::string>
{
    using StrongType::StrongType;

    static XmrChannel fromCmsSettings(const std::string& host,
                                      const std::string& cmsKey,
                                      const std::string& hardwareKey);
};
