#pragma once

#include "control/common/XlfResources.hpp"

namespace ResourcesXlf::Player
{
    const std::string Mute = ResourcesXlf::option("mute");
    const std::string Loop = ResourcesXlf::option("loop");
    const std::string Volume = ResourcesXlf::option("volume");
}

namespace ResourcesXlf::AudioNode
{
    const std::string Id = "mediaId";
    const std::string Uri = "uri";
    const std::string Loop = ResourcesXlf::attr("loop");
    const std::string Volume = ResourcesXlf::attr("volume");
}
