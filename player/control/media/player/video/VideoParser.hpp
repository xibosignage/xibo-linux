#pragma once

#include "control/media/MediaParser.hpp"

class VideoParser : public MediaParser
{
protected:
    ExtraOptions parseExtraOptionsImpl(const xml_node& node) override;
    MediaGeometry geometryFrom(const xml_node& node) override;

};
