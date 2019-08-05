#pragma once

#include "control/media/MediaFactory.hpp"

class IImage;

class ImageFactory : public MediaFactory
{
protected:
    std::unique_ptr<IMedia> create(const MediaOptions& baseOptions, const ExtraOptions& options) override;

private:
    std::shared_ptr<IImage> createView(const Uri& uri, int width, int height, MediaGeometry::ScaleType scaleType);
};
