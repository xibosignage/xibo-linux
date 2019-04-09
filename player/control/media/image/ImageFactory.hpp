#pragma once

#include "control/media/MediaFactory.hpp"

#include "ImageOptions.hpp"

class ImageFactory : public VisibleMediaFactory
{
public:
    ImageFactory(const ImageOptions& options);

    std::unique_ptr<Media> createModel(const std::shared_ptr<Widget>& view) override;
    ViewInfo createView(int width, int height) override;

private:
    ImageOptions m_options;

};
