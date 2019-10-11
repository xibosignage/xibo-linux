#pragma once

#include "control/media/Media.hpp"
#include "control/media/MediaOptions.hpp"
#include "control/widgets/Image.hpp"

#include <memory>

class ImageFactory
{
public:
    std::unique_ptr<Xibo::Media> create(const MediaOptions& baseOptions, int width, int height);

private:
    std::shared_ptr<Xibo::Image> createWidget(const Uri& uri,
                                              int width,
                                              int height,
                                              MediaGeometry::ScaleType scaleType);
};
