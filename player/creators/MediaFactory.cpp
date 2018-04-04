#include "MediaFactory.hpp"

#include "media/Image.hpp"
#include "media/Video.hpp"
#include "media/WebView.hpp"
#include "media/Audio.hpp"

std::unique_ptr<Media> MediaFactory::create(const Region& region, const Params& params)
{
    auto type = params.get<MediaType>("type");
    switch(type)
    {
    case MediaType::Image:
        return std::make_unique<Image>(region, params.get<int>("id"),  params.get<int>("duration"),  params.get<std::string>("uri"),
                                       params.get<std::string>("scaleType"), params.get<std::string>("align"), params.get<std::string>("valign"));
    case MediaType::Video:
        return std::make_unique<Video>(region, params.get<int>("id"), params.get<int>("duration"), params.get<std::string>("uri"),
                                       params.get<bool>("mute"), params.get<bool>("loop"));
    case MediaType::WebView:
        return std::make_unique<WebView>(region, params.get<int>("id"), params.get<int>("duration"), params.get<std::string>("uri"),
                                         params.get<int>("modeId"), params.get<bool>("transparency"));
    case MediaType::Audio:
        return std::make_unique<Audio>(region, params.get<int>("id"), params.get<int>("duration"), params.get<std::string>("uri"),
                                       params.get<bool>("mute"), params.get<bool>("loop"));
    }

    return nullptr;
}
