#pragma once

#include "MediaOptions.hpp"
#include <memory>

class IMedia;
class TransitionExecutor;

class MediaFactory
{
public:
    virtual ~MediaFactory() = default;
    virtual std::unique_ptr<IMedia> create(const MediaOptions& options, const ExtraOptions& extraOptions) = 0;
};
