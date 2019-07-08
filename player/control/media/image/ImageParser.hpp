#pragma once

#include "control/media/creators/MediaParser.hpp"

class ImageParser : public MediaParser
{
protected:
    MediaGeometry geometry() override;
    ExtraOptions parseAdditonalOptions(const xml_node& node) override;

};
