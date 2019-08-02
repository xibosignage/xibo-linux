#pragma once

#include "control/media/MediaParser.hpp"

class ImageParser : public MediaParser
{
protected:
    MediaGeometry geometryFrom(const xml_node& node) override;
    ExtraOptions parseExtraOptionsImpl(const xml_node& node) override;

};
