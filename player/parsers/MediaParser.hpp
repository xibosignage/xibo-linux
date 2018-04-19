#pragma once

#include "constants.hpp"
#include "ParserHelpers.hpp"
#include "utils/utilities.hpp"

#include <spdlog/spdlog.h>
#include <any>

class MediaParser
{
private:
    MediaParser(const xlf_node& media_node);
    AnyMedia parse_media();
    template<MediaType type> AnyMedia parse_media_params(const xlf_node& media_node);
    template<MediaType type> void parse_media_options(ParsedMedia<type>& media_params, const xlf_node& options);
    boost::optional<int> parse_duration(const std::string& path);
    std::shared_ptr<ParsedMedia<MediaType::Audio>> parse_audio_node(const xlf_node& audio_node);

    friend class RegionParser;

    std::string get_path(int id, const boost::optional<std::string>& uri);
    MediaType to_media_type(const std::string& type);

private:
    const xlf_node& m_media_node;

};
