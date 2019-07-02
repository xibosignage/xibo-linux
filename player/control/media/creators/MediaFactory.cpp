#include "MediaFactory.hpp"
#include "MediaFactoriesRepo.hpp"

#include "../IMedia.hpp"

std::unique_ptr<IMedia> MediaFactory::create(const ParsedMedia& parsed)
{
    auto media = createImpl(parsed.baseOptions, parsed.extraOptions);

    if(parsed.attachedMedia)
    {
        attachAdditionalMedia(*media, *parsed.attachedMedia);
    }

    return media;
}

void MediaFactory::attachAdditionalMedia(IMedia& media, const ParsedMedia& additionalMedia)
{
    auto&& factory = MediaFactoriesRepo::get(additionalMedia.baseOptions.type);

    if(factory)
    {
        media.attachMedia(factory->create(additionalMedia));
    }
}
