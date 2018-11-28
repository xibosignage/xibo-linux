#pragma once

#include "IBackground.hpp"
#include <memory>

class FilePath;

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
    OneColorBackground(int width, int height, uint32_t color, std::unique_ptr<IImageAdaptor>&& handler);

};

class ImageBackground : public Background
{
public:
    ImageBackground(int width, int height, const FilePath& path, std::unique_ptr<IImageAdaptor>&& handler);

};
