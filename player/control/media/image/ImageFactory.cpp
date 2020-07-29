#include "ImageFactory.hpp"

#include "control/media/MediaImpl.hpp"
#include "control/widgets/Image.hpp"

std::unique_ptr<Xibo::Media> ImageFactory::create(const MediaOptions& baseOptions, int width, int height)
{
    auto media = std::make_unique<MediaImpl>(baseOptions);
    media->setWidget(createWidget(baseOptions.uri, width, height, baseOptions.geometry.scaleType));
    return media;
}

std::shared_ptr<Xibo::Image> ImageFactory::createWidget(const Uri& uri,
                                                        int width,
                                                        int height,
                                                        MediaGeometry::ScaleType scaleType)
{
    bool isScaled = scaleType == MediaGeometry::ScaleType::Scaled ? true : false;
    return ImageWidgetFactory::create(uri, width, height, static_cast<Xibo::Image::PreserveRatio>(isScaled));
}
