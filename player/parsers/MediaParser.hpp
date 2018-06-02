#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>

class Media;

class MediaParser
{
public:
    MediaParser(const xlf_node& parent_node, const xlf_node& media_node);
    virtual ~MediaParser() = default;
    virtual std::unique_ptr<Media> parse() = 0;

protected:
    const xlf_node& attrs() const;
    const xlf_node& options() const;
    const xlf_node& parent_node() const;
    std::unique_ptr<Media> parse_audio_node();

private:
    const xlf_node& m_parent_node;
    xlf_node m_attrs;
    xlf_node m_options;
    boost::optional<xlf_node> m_audio_node;

};
