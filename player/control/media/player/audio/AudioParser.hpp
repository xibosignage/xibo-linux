#pragma once

#include "control/media/MediaParser.hpp"

class AudioParser : public MediaParser
{
public:
    std::unique_ptr<IMedia> mediaFrom(const ptree_node& node) override;

protected:
    ExtraOptions parseExtraOptionsImpl(const ptree_node& node) override;

private:
    std::unique_ptr<IMedia> parseAdditionalNode(const ptree_node& node);

};
