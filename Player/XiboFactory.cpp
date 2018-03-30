#include "XiboFactory.hpp"

#include "VideoHandler.hpp"
#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include "WebView.hpp"

std::unique_ptr<MainLayout> XlfFactory::create_layout(const Params& params)
{
    auto layout = std::make_unique<MainLayout>(params.get<int>("schemaVersion"),
                                               params.get<int>("width"),
                                               params.get<int>("height"),
                                               params.get<std::string>("background"),
                                               params.get<std::string>("bgcolor"));

    auto regions = params.get_child("regions");
    for(auto [region_id, region_params] : regions)
    {
        layout->add_region(XlfFactory::create_region(region_params));
    }
    return layout;
}

std::unique_ptr<Region> XlfFactory::create_region(const Params& params)
{
    auto region = std::make_unique<Region>(params.get<int>("id"),
                                           Size{params.get<int>("width"), params.get<int>("height")},
                                           Point{params.get<int>("left"), params.get<int>("top")},
                                           params.get<int>("zindex"),
                                           params.get<bool>("loop"));

    auto media = params.get_child("medias");
    for(auto [media_id, media_params] : media)
    {
        region->add_media(XlfFactory::create_media(region->size(), media_params));
    }
    return region;
}

std::unique_ptr<Media> XlfFactory::create_media(const Size& size, const Params& params)
{
    auto type = params.get<std::string>("type");
    if(type == "image")
    {
        return create_image(size, params);
    }
    else if(type == "video")
    {
        return create_video(size, params);
    }
    else
    {
        return create_webview(size, params);
    }

}

std::unique_ptr<Media> XlfFactory::create_image(const Size& size, const Params& params)
{
    return std::make_unique<Image>(size,
                                   params.get<int>("id"),
                                   params.get<int>("duration"),
                                   params.get<bool>("useDuration"),
                                   params.get<std::string>("uri"),
                                   params.get<std::string>("scaleType"),
                                   params.get<std::string>("align"),
                                   params.get<std::string>("valign"));
}

std::unique_ptr<Media> XlfFactory::create_video(const Size& size, const Params& params)
{
    return std::make_unique<Video>(size,
                                   params.get<int>("id"),
                                   params.get<int>("duration"),
                                   params.get<bool>("useDuration"),
                                   params.get<std::string>("uri"),
                                   params.get<bool>("mute"),
                                   params.get<bool>("loop"));
}

std::unique_ptr<Media> XlfFactory::create_webview(const Size& size, const Params& params)
{
    return std::make_unique<WebView>(size,
                                     params.get<int>("id"),
                                     params.get<int>("duration"),
                                     params.get<bool>("useDuration"),
                                     params.get<std::string>("uri"),
                                     params.get<int>("modeId"),
                                     params.get<bool>("transparency"));
}
