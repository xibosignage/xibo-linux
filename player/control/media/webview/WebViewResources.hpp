#pragma once

#include "common/parsing/Parsing.hpp"
#include "control/XlfResources.hpp"

namespace XlfResources::WebView
{
    const std::string Transparency = Parsing::xmlOption("transparency");
    const std::string ModeId = Parsing::xmlOption("modeid");
}
