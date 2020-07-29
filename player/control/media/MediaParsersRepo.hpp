#pragma once

#include "common/Repository.hpp"

#include "control/media/MediaOptions.hpp"
#include "control/media/MediaParser.hpp"
#include "control/media/MediaResources.hpp"

class MediaParsersRepo
{
public:
    static void init();

    static boost::optional<MediaParser&> get(const MediaOptions::Type& mediaType)
    {
        // TODO fix while adding plugging support
        if (mediaType.type != XlfResources::Media::HlsType && mediaType.render == XlfResources::Media::HtmlRender)
        {
            return parsers_.get(XlfResources::Media::HtmlRender);
        }
        return parsers_.get(mediaType.type + mediaType.render);
    }

    static void add(const MediaOptions::Type& mediaType, std::unique_ptr<MediaParser>&& parser)
    {
        parsers_.add(mediaType.type + mediaType.render, std::move(parser));
    }

private:
    static Repository<MediaParser> parsers_;
};
