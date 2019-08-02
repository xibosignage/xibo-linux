#pragma once

#include "common/Repository.hpp"

#include "MediaResources.hpp"
#include "MediaParser.hpp"
#include "MediaFactory.hpp"
#include "MediaOptions.hpp"

#include "common/logger/Logging.hpp"

class MediaCreatorsRepo
{
public:
    static void init();

    template<typename Creator>
    static boost::optional<Creator&> get(const MediaOptions::Type& mediaType)
    {
        if constexpr(std::is_base_of<Creator, MediaParser>::value)
        {
            return get(parsers, mediaType);
        }
        else
        {
            return get(factories, mediaType);
        }
    }

    template<typename Parser, typename Factory>
    static void add(const MediaOptions::Type& mediaType, std::unique_ptr<Parser>&& parser, std::unique_ptr<Factory>&& factory)
    {
        parsers.add(mediaType.type + mediaType.render, std::move(parser));
        factories.add(mediaType.type + mediaType.render, std::move(factory));
    }

private:
    template<typename Creator>
    static boost::optional<Creator&> get(const Repository<Creator>& repo, const MediaOptions::Type& mediaType)
    {
        // FIXME while adding plugging support
        if(mediaType.type != ResourcesXlf::Media::HlsType && mediaType.render == ResourcesXlf::Media::HtmlRender)
        {
            return repo.get(ResourcesXlf::Media::HtmlRender);
        }
        return repo.get(mediaType.type + mediaType.render);
    }

private:
    static Repository<MediaParser> parsers;
    static Repository<MediaFactory> factories;

};
