#pragma once

#include "common/Repository.hpp"

#include "MediaResources.hpp"
#include "../MediaOptions.hpp"

template<typename RepoType, typename StoredType>
class MediaRepo : public Repository<RepoType, StoredType>
{
public:
    static boost::optional<StoredType&> get(const MediaOptions::Type& mediaType)
    {
        // FIXME while adding plugging support
        if(mediaType.type != ResourcesXlf::Media::HlsType && mediaType.render == ResourcesXlf::Media::HtmlRender)
        {
            return Repository<RepoType, StoredType>::get(ResourcesXlf::Media::HtmlRender);
        }
        return Repository<RepoType, StoredType>::get(mediaType.type + mediaType.render);
    }

    static void add(const MediaOptions::Type& mediaType, std::unique_ptr<StoredType>&& object)
    {
        Repository<RepoType, StoredType>::add(mediaType.type + mediaType.render, std::move(object));
    }

};
