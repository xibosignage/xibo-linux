#pragma once

#include "IWidgetAdaptor.hpp"
#include "constants.hpp"

using OnVideoFinished = std::function<void()>;

class IVideoHandler : public IWidgetAdaptor
{
public:
    virtual ~IVideoHandler() = default;

    virtual void load(const FilePath& path) = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void connect(OnVideoFinished callback) = 0;
    virtual void setVolume(double volume) = 0;
};
