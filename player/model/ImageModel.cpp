#include "ImageModel.hpp"

ImageModel::ImageModel(const ImageOptions& options) :
    MediaModel(options),
    m_geometry(MediaGeometry{options.scaleType, options.align, options.valign})
{
}

bool ImageModel::isScaled() const
{
    return m_geometry.scaleType == MediaGeometry::ScaleType::Scaled ? true : false;
}

MediaGeometry::Align ImageModel::align() const
{
    return m_geometry.align;
}

MediaGeometry::Valign ImageModel::valign() const
{
    return m_geometry.valign;
}

void ImageModel::apply(IMediaVisitor& visitor)
{
    visitor.visit(shared_from_this());
}
