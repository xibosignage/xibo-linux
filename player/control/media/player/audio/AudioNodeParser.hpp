#pragma once

#include "control/media/MediaParser.hpp"

class AudioNodeParser : public MediaParser
{
protected:
    MediaOptions::Type typeFrom(const ptree_node& node) override;
    int idFrom(const ptree_node& node) override;
    Uri uriFrom(const ptree_node& node) override;
    int durationFrom(const ptree_node& node) override;
    ExtraOptions extraOptionsImpl(const ptree_node& node) override;
};
