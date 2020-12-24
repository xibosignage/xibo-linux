#pragma once

#include "common/parsing/Parsing.hpp"
#include "control/XlfResources.hpp"

namespace XlfResources::Player
{
    const std::string Mute = Parsing::xmlOption("mute");
    const std::string Loop = Parsing::xmlOption("loop");
    const std::string Volume = Parsing::xmlOption("volume");
}

namespace XlfResources::AudioNode
{
    const std::string Id = Parsing::xmlAttr("mediaId");
    const std::string Uri = "uri";
    const std::string Loop = Parsing::xmlAttr("loop");
    const std::string Volume = Parsing::xmlAttr("volume");
}
