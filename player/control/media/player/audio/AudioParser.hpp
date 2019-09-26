#pragma once

#include "control/media/MediaParser.hpp"

class AudioParser : public MediaParser
{
protected:
    ExtraOptions extraOptionsImpl(const ptree_node& node) override;

private:
    std::unique_ptr<IMedia> parseAdditionalNode(const ptree_node& node);
};
