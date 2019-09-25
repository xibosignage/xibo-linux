#pragma once

#include "common/Parsing.hpp"
#include "control/common/XlfResources.hpp"

namespace XlfResources::Player
{
const std::string Mute = XlfResources::option("mute");
const std::string Loop = XlfResources::option("loop");
const std::string Volume = XlfResources::option("volume");
}

namespace XlfResources::AudioNode
{
const std::string Id = Parsing::xmlAttr("mediaId");
const std::string Uri = "uri";
const std::string Loop = Parsing::xmlAttr("loop");
const std::string Volume = Parsing::xmlAttr("volume");
}
