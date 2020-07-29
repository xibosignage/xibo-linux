#pragma once

#include "control/media/MediaParser.hpp"

class VideoParser : public MediaParser
{
protected:
    MediaGeometry geometryFrom(const XmlNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& baseOptions,
                                             const XmlNode& node,
                                             int width,
                                             int height) override;
};
