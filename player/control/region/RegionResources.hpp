#pragma once

#include "common/parsing/Parsing.hpp"
#include "control/XlfResources.hpp"

namespace XlfResources::Region
{
    const std::string Id = Parsing::xmlAttr("id");
    const std::string Width = Parsing::xmlAttr("width");
    const std::string Height = Parsing::xmlAttr("height");
    const std::string Left = Parsing::xmlAttr("left");
    const std::string Top = Parsing::xmlAttr("top");
    const std::string Zindex = Parsing::xmlAttr("zindex");
    const std::string Loop = Parsing::xmlOption("loop");
}
