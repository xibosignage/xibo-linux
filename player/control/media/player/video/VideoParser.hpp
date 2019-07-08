#pragma once

#include "control/media/creators/MediaParser.hpp"

class VideoParser : public MediaParser
{
protected:
    ExtraOptions parseAdditonalOptions(const xml_node& node) override;
    MediaGeometry geometry() override;

};
