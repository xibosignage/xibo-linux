#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>

class Media;

class MediaParser
{
public:
    MediaParser(const xlf_node& parent_node, const xlf_node& media_node);
    virtual ~MediaParser() = default;
    std::unique_ptr<Media> parse();

protected:
    const xlf_node& attrs() const;
    const xlf_node& options() const;
    const xlf_node& parent_node() const;
    virtual std::unique_ptr<Media> doParse() = 0;

private:
    std::unique_ptr<Media> parse_audio_node(int parent_duration);

private:
    const xlf_node& m_parent_node;
    xlf_node m_attrs;
    xlf_node m_options;
    boost::optional<const xlf_node&> m_audio_node;

};
