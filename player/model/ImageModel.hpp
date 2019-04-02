#pragma once

#include "MediaModel.hpp"
#include "MediaGeometry.hpp"

class ImageModel : public MediaModel, public std::enable_shared_from_this<ImageModel>
{
public:
    ImageModel(const ImageOptions& options);

    bool isScaled() const;
    MediaGeometry::Align align() const;
    MediaGeometry::Valign valign() const;

    void apply(IMediaVisitor& visitor) override;

private:
    MediaGeometry m_geometry;

};
