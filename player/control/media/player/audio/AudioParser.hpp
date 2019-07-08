#pragma once

#include "control/media/creators/MediaParser.hpp"

class AudioParser : public MediaParser
{
public:
    ParsedMedia parse(const xml_node& node) override;

protected:
    ExtraOptions parseAdditonalOptions(const xml_node& node) override;

private:
    ParsedMedia parseAdditionalNode(const xml_node& node);

};
