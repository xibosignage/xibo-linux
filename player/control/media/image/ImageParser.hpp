#pragma once

#include "control/media/MediaParser.hpp"

class ImageParser : public MediaParser
{
protected:
    MediaGeometry geometryFrom(const ptree_node& node) override;
    ExtraOptions parseExtraOptionsImpl(const ptree_node& node) override;

};
