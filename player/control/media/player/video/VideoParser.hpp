#pragma once

#include "control/media/MediaParser.hpp"

class VideoParser : public MediaParser
{
protected:
    ExtraOptions parseExtraOptionsImpl(const ptree_node& node) override;
    MediaGeometry geometryFrom(const ptree_node& node) override;

};
