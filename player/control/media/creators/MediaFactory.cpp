#include "MediaFactory.hpp"
#include "MediaFactoriesRepo.hpp"

#include "../IMedia.hpp"
#include "../../common/transitions/FlyTransitionExecutor.hpp"
#include "../../common/transitions/FadeTransitionExecutor.hpp"

std::unique_ptr<IMedia> MediaFactory::create(const ParsedMedia& parsed)
{
    auto media = createImpl(parsed.baseOptions, parsed.extraOptions);

    media->setInTransition(createTransition<Transition::Heading::In>(*media, parsed.baseOptions.inTrans));
    media->setOutTransition(createTransition<Transition::Heading::Out>(*media, parsed.baseOptions.outTrans));

    if(parsed.attachedMedia)
    {
        attachAdditionalMedia(*media, *parsed.attachedMedia);
    }

    return media;
}


template<Transition::Heading heading>
std::unique_ptr<TransitionExecutor> MediaFactory::createTransition(IMedia& media, const boost::optional<Transition>& transition)
{
    if(transition)
    {
        switch(transition->type)
        {
            case Transition::Type::Fly:
                return std::make_unique<FlyTransitionExecutor>(heading, transition->direction, transition->duration, media.view());
            case Transition::Type::Fade:
                return std::make_unique<FadeTransitionExecutor>(heading, transition->duration, media.view());
        }
    }

    return nullptr;
}

void MediaFactory::attachAdditionalMedia(IMedia& media, const ParsedMedia& additionalMedia)
{
    auto&& factory = MediaFactoriesRepo::get(additionalMedia.baseOptions.type);

    if(factory)
    {
        media.attachMedia(factory->create(additionalMedia));
    }
}
