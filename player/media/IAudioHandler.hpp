#pragma once

#include "constants.hpp"

using OnAudioFinished = std::function<void()>;

class IAudioHandler
{
public:
    virtual ~IAudioHandler() = default;

    virtual void load(const FilePath& path) = 0;
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void connect(OnAudioFinished callback) = 0;
    virtual void setVolume(double volume) = 0;
};
