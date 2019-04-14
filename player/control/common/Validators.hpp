#pragma once

#include "common/uri/Uri.hpp"

namespace Validators
{
    Uri validateUri(const boost::optional<std::string>& uri);
    uint32_t validateColor(const std::string& color);
}
