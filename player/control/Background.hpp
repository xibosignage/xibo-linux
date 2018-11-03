#pragma once

#include "IBackground.hpp"
#include "constants.hpp"
#include "adaptors/IImageAdaptor.hpp"

#include <memory>

const bool DEFAULT_BACKGROUND_SCALED = true;

class Background : public IBackground
{
public:
    Background(const Background&) = delete;
    Background& operator=(const Background&) = delete;

    int width() const override;
    int height() const override;
    void scale(double scaleX, double scaleY) override;

    void show() override;
    IImageAdaptor& handler() override;

protected:
    Background(int width, int height, std::unique_ptr<IImageAdaptor>&& handler);

private:
    std::unique_ptr<IImageAdaptor> m_handler;

};

class OneColorBackground : public Background
{
public:
    OneColorBackground(int width, int height, uint32_t color, std::unique_ptr<IImageAdaptor>&& handler) :
        Background(width, height, std::move(handler))
    {
        this->handler().setColor(color);
    }

};

class ImageBackground : public Background
{
public:
    ImageBackground(int width, int height, const FilePath& path, std::unique_ptr<IImageAdaptor>&& handler) :
        Background(width, height, std::move(handler))
    {
        this->handler().loadImage(path, DEFAULT_BACKGROUND_SCALED);
    }

};
