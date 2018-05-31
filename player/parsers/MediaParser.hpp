#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>

class Media;

class MediaParser
{
public:
    MediaParser() = default;
    virtual std::unique_ptr<Media> parse() = 0;
};

//std::shared_ptr<ParsedMedia<MediaType::Audio>> MediaParser::parse_audio_node(const xlf_node& audio_node)
//{
//    auto uri_node = audio_node.get_child("uri");
//    auto attrs = uri_node.get_child("<xmlattr>");

//    auto audio = std::make_shared<ParsedMedia<MediaType::Audio>>();
//    audio->id = attrs.get<int>("mediaId");
//    audio->uri = utilities::example_dir() + "/" + uri_node.get_value<std::string>();
//    audio->mute = attrs.get_optional<bool>("mute");
//    audio->loop = attrs.get_optional<bool>("loop");
//    audio->volume = attrs.get_optional<int>("volume");
//    return audio;
//}
