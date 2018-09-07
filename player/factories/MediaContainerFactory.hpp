#pragma once

#include "constants.hpp"
#include <boost/property_tree/ptree.hpp>

class IMediaContainer;

class MediaContainerFactory
{
public:
    MediaContainerFactory(const xlf_node& regionNode);
    std::unique_ptr<IMediaContainer> create();

private:
    xlf_node m_mediaContainerNode;

};
