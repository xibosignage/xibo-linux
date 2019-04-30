#pragma once

#include "control/media/MediaFactory.hpp"
#include "ImageOptions.hpp"

class Widget;

class ImageFactory : public MediaFactory
{
public:
    ImageFactory(int width, int height, const ImageOptions& options);

    std::unique_ptr<Media> create() override;

private:
    std::shared_ptr<Widget> createView(int width, int height);

private:
    ImageOptions m_options;
    int m_width;
    int m_height;
};
