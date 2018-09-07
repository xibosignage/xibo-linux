#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>

class IMedia;

class MediaFactory
{
public:
    MediaFactory(const xlf_node& mediaNode);
    virtual ~MediaFactory() = default;

    std::unique_ptr<IMedia> create();

protected:
    const xlf_node& attrs() const;
    const xlf_node& options() const;
    virtual std::unique_ptr<IMedia> doCreate() = 0;

private:
    std::unique_ptr<IMedia> createAudioNode(int parentDuration);

private:
    xlf_node m_attrs;
    xlf_node m_options;
    boost::optional<const xlf_node&> m_audioNode;

};
