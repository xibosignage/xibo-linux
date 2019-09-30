#pragma once

#include "control/media/MediaParser.hpp"

class VideoParser : public MediaParser
{
protected:
    MediaGeometry geometryFrom(const PtreeNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& baseOptions,
                                             const PtreeNode& node,
                                             int width,
                                             int height) override;
};
