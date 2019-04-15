#pragma once

#include "control/media/MediaParser.hpp"

#include "ImageOptions.hpp"

class ImageParser : public MediaParser
{
public:
    ImageParser(const xml_node& node);
    ImageOptions parse();

protected:
    MediaGeometry geometry() override;

};
