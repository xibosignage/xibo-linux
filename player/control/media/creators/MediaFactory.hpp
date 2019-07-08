#pragma once

#include "ParsedMedia.hpp"

class IMedia;
class TransitionExecutor;

class MediaFactory
{
public:
    virtual ~MediaFactory() = default;
    std::unique_ptr<IMedia> create(const ParsedMedia& parsed);

protected:
    virtual std::unique_ptr<IMedia> createImpl(const MediaOptions& baseOptions, const ExtraOptions& options) = 0;

private:
    void attachAdditionalMedia(IMedia& media, const ParsedMedia& additionalMedia);

    template<Transition::Heading heading>
    std::unique_ptr<TransitionExecutor> createTransition(IMedia& media, const boost::optional<Transition>& transition);

};
