#pragma once

#include "IBackground.hpp"

#include <memory>
#include <boost/noncopyable.hpp>

class Uri;

class Background : public IBackground, private boost::noncopyable
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
    friend class BackgroundBuilder;

    OneColorBackground(int width, int height, uint32_t color, std::unique_ptr<IImageAdaptor>&& handler);

};

class ImageBackground : public Background
{
    friend class BackgroundBuilder;

    ImageBackground(int width, int height, const Uri& uri, std::unique_ptr<IImageAdaptor>&& handler);
};
