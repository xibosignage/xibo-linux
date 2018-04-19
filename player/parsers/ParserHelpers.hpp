#pragma once

#include <string>
#include <vector>
#include <any>
#include "constants.hpp"

template <MediaType type>
struct ParsedMedia;

struct ParsedBaseMedia
{
    int id;
    std::string uri;
    int duration;
    std::shared_ptr<ParsedMedia<MediaType::Audio>> audio;
};

struct AnyMedia : std::any
{
    using std::any::any;
    MediaType media_type;
};

template<>
struct ParsedMedia<MediaType::Image> : ParsedBaseMedia
{
    boost::optional<std::string> scale_type;
    boost::optional<std::string> align;
    boost::optional<std::string> valign;
};

template<>
struct ParsedMedia<MediaType::Video> : ParsedBaseMedia
{
    boost::optional<bool> mute;
    boost::optional<bool> loop;
};

template<>
struct ParsedMedia<MediaType::Audio> : ParsedBaseMedia
{
    boost::optional<bool> mute;
    boost::optional<bool> loop;
    boost::optional<int> volume;
};

template<>
struct ParsedMedia<MediaType::WebView> : ParsedBaseMedia
{
    boost::optional<int> mode_id;
    boost::optional<bool> transparency;
};

struct ParsedRegion
{
    int id;
    int width;
    int height;
    int top;
    int left;
    boost::optional<int> zindex;
    boost::optional<bool> loop;
    std::vector<AnyMedia> media;
};

struct ParsedLayout
{
    int schemaVersion;
    int width;
    int height;
    boost::optional<std::string> bgimage;
    boost::optional<std::string> bgcolor;
    std::vector<ParsedRegion> regions;
};
