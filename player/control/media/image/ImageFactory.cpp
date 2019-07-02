#include "ImageFactory.hpp"

#include "control/common/Image.hpp"
#include "control/media/Media.hpp"
#include "control/region/RegionResources.hpp"

#include "common/FilePath.hpp"

std::unique_ptr<IMedia> ImageFactory::createImpl(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    int width = std::stoi(options.at(ResourcesXlf::Region::Width));
    int height = std::stoi(options.at(ResourcesXlf::Region::Height));

    auto view = createView(baseOptions.uri, width, height, baseOptions.geometry.scaleType);
    return std::make_unique<Media>(baseOptions, view);
}

std::shared_ptr<IImage> ImageFactory::createView(const Uri& uri, int width, int height, MediaGeometry::ScaleType scaleType)
{
    auto image = std::make_shared<Image>(width, height);

    bool isScaled = scaleType == MediaGeometry::ScaleType::Scaled ? true : false;
    image->loadFromFile(uri.path(), isScaled);

    return image;
}
