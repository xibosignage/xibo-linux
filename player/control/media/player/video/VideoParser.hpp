#pragma once

#include "control/media/MediaParser.hpp"

class VideoParser : public MediaParser
{
protected:
    ExtraOptions extraOptionsImpl(const ptree_node& node) override;
    MediaGeometry geometryFrom(const ptree_node& node) override;
};
