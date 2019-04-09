#include "ImageFactory.hpp"

#include "control/common/Image.hpp"
#include "control/media/VisibleMedia.hpp"

ImageFactory::ImageFactory(const ImageOptions& options) :
    m_options(options)
{
}

std::unique_ptr<Media> ImageFactory::createModel(const std::shared_ptr<Widget>& view)
{
    return std::make_unique<VisibleMedia>(m_options, *view);
}

ViewInfo ImageFactory::createView(int width, int height)
{
    auto image = std::make_shared<Image>(width, height);
    bool isScaled = m_options.geometry.scaleType == MediaGeometry::ScaleType::Scaled ? true : false;
    image->loadFromFile(m_options.uri, isScaled);
    return {image, m_options.geometry.align, m_options.geometry.valign};
}
