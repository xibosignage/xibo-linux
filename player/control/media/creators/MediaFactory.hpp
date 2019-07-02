#pragma once

#include "ParsedMedia.hpp"

class IMedia;

class MediaFactory
{
public:
    virtual ~MediaFactory() = default;
    std::unique_ptr<IMedia> create(const ParsedMedia& parsed);

protected:
    virtual std::unique_ptr<IMedia> createImpl(const MediaOptions& baseOptions, const ExtraOptions& options) = 0;

private:
    void attachAdditionalMedia(IMedia& media, const ParsedMedia& additionalMedia);

};
