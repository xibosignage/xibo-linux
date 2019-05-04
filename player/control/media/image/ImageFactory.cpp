#include "ImageFactory.hpp"

#include "control/common/Image.hpp"
#include "common/FilePath.hpp"

ImageFactory::ImageFactory(int width, int height, const ImageOptions& options) :
    m_options(options),
    m_width(width),
    m_height(height)
{
}

std::unique_ptr<Media> ImageFactory::create()
{
    return std::make_unique<Media>(m_options, createView(m_width, m_height));
}

std::shared_ptr<Widget> ImageFactory::createView(int width, int height)
{
    auto image = std::make_shared<Image>(width, height);

    bool isScaled = m_options.geometry.scaleType == MediaGeometry::ScaleType::Scaled ? true : false;
    image->loadFromFile(m_options.uri.path(), isScaled);

    return image;
}
