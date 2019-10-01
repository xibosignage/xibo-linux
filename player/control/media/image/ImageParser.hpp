#pragma once

#include "control/media/MediaParser.hpp"

class ImageParser : public MediaParser
{
protected:
    MediaGeometry geometryFrom(const PtreeNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& options,
                                             const PtreeNode& node,
                                             int width,
                                             int height) override;
};
