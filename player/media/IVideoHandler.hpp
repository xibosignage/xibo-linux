#pragma once

#include "constants.hpp"

using OnVideoFinished = std::function<void()>;

class IWidgetAdaptor;
class FilePath;

class IVideoHandler
{
public:
    virtual ~IVideoHandler() = default;

    virtual IWidgetAdaptor& videoWindow() =0;

    virtual void scale(double scaleX, double scaleY) = 0;
    virtual void setSize(int width, int height) = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual void load(const FilePath& path) = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void stopPlayback() = 0;
    virtual void connect(OnVideoFinished callback) = 0;
    virtual void setVolume(int volume) = 0;
};
