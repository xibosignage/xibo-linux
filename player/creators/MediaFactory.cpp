#include "MediaFactory.hpp"

#include "media/Image.hpp"
#include "media/Video.hpp"
#include "media/WebView.hpp"
#include "media/Audio.hpp"
#include "control/Region.hpp"

#include "parsers/MediaParser.hpp"

std::unique_ptr<Media> MediaFactory::create(const Region& region, int media_id)
{
    std::unique_ptr<Media> media;
    MediaParser parser;

    auto params = parser.parse_media(region.id(), media_id);
    auto type = params.get<MediaType>("type");

    switch(type)
    {
    case MediaType::Image:
        media = std::make_unique<Image>(region, params.get<int>("id"),  params.get<int>("duration"),  params.get<std::string>("uri"),
                                       params.get<std::string>("scaleType"), params.get<std::string>("align"), params.get<std::string>("valign"));
        break;
    case MediaType::Video:
        media = std::make_unique<Video>(region, params.get<int>("id"), params.get<int>("duration"), params.get<std::string>("uri"),
                                        params.get<bool>("mute"), params.get<bool>("loop"));
        break;
    case MediaType::WebView:
        media = std::make_unique<WebView>(region, params.get<int>("id"), params.get<int>("duration"), params.get<std::string>("uri"),
                                          params.get<int>("modeId"), params.get<bool>("transparency"));
        break;
    case MediaType::Audio:
        media = std::make_unique<Audio>(region, params.get<int>("id"), params.get<int>("duration"), params.get<std::string>("uri"),
                                        params.get<bool>("mute"), params.get<bool>("loop"));
        break;
    }

//    boost::property_tree::ptree empty{};
//    if(auto&& audio = create(region, params.get_child_optional("audio").value_or(empty)))
//    {
//        spdlog::get(LOGGER)->debug("audio module exists");
//        media->attach_audio(std::move(audio));
//    }

    return media;
}
