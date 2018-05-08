#include "MediaFactory.hpp"

#include "media/Image.hpp"
#include "media/Video.hpp"
#include "media/WebView.hpp"
#include "media/Audio.hpp"
#include "control/Region.hpp"

#include "parsers/ParserHelpers.hpp"
#include "parsers/MediaParser.hpp"

Image::ScaleType to_scale_type(const std::string& scale_type);
Image::Align to_align(const std::string& align);
Image::Valign to_valign(const std::string& valign);

const int MAX_VOLUME = 100;

std::unique_ptr<Media> MediaFactory::create(const Region& region, const AnyMedia& object)
{    
    switch(object.media_type)
    {
        case MediaType::Image:
        {
            auto parsed_image = std::any_cast<ParsedMedia<MediaType::Image>>(object);
            return create_media<MediaType::Image>(region, parsed_image);
        }
        case MediaType::Video:
        {
            auto parsed_video = std::any_cast<ParsedMedia<MediaType::Video>>(object);
            return create_media<MediaType::Video>(region, parsed_video);
        }
        case MediaType::WebView:
        {
            auto parsed_webview = std::any_cast<ParsedMedia<MediaType::WebView>>(object);
            return create_media<MediaType::WebView>(region, parsed_webview);
        }
        case MediaType::Audio:
        {
            auto parsed_audio = std::any_cast<ParsedMedia<MediaType::Audio>>(object);
            return create_media<MediaType::Audio>(region, parsed_audio);
        }
    }
    return nullptr;
}

template<MediaType type>
std::unique_ptr<Media> MediaFactory::create_media(const Region& region, const ParsedMedia<type>& object)
{
    std::unique_ptr<Media> media;

    int id = object.id;
    std::string uri = object.uri;
    int duration = object.duration;

    if constexpr(type == MediaType::Image)
    {
        auto scale_type = to_scale_type(object.scale_type.value_or("center"));
        auto align = to_align(object.align.value_or("center"));
        auto valign = to_valign(object.valign.value_or("middle"));

        media = std::make_unique<Image>(region, id, duration, uri, scale_type, align, valign);
    }
    else if constexpr(type == MediaType::Video)
    {
        auto mute = object.mute.value_or(false);
        auto loop = object.loop.value_or(false);

        media = std::make_unique<Video>(region, id, duration, uri, mute, loop);
    }
    else if constexpr(type == MediaType::WebView)
    {
        auto mute = object.mode_id.value_or(-1);
        auto loop = object.transparency.value_or(true);

        media = std::make_unique<WebView>(region, id, duration, uri, mute, loop);
    }
    else if constexpr(type == MediaType::Audio)
    {
        auto mute = object.mute.value_or(false);
        auto loop = object.loop.value_or(false);
        auto volume = object.volume.value_or(MAX_VOLUME) / static_cast<double>(MAX_VOLUME);

        media = std::make_unique<Audio>(region, id, duration, uri, mute, loop, volume);
    }

    if(object.audio)
    {
        auto&& audio = create_media<MediaType::Audio>(region, *object.audio);
        spdlog::get(LOGGER)->debug("audio module exist");
        media->attach_audio(std::move(audio));
    }

    return media;
}

Image::ScaleType to_scale_type(const std::string& scale_type)
{
    if(scale_type == "center")
        return Image::ScaleType::Scaled;
    else if(scale_type == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align to_align(const std::string& align)
{
    if(align == "left")
        return Image::Align::Left;
    else if(align == "center")
        return Image::Align::Center;
    else if(align == "right")
        return Image::Align::Right;
    else
        return Image::Align::Invalid;
}

Image::Valign to_valign(const std::string& valign)
{
    if(valign == "top")
        return Image::Valign::Top;
    else if(valign == "middle")
        return Image::Valign::Middle;
    else if(valign == "bottom")
        return Image::Valign::Bottom;
    else
        return Image::Valign::Invalid;
}

