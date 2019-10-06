#pragma once

#include "control/media/MediaParser.hpp"

class AudioParser : public MediaParser
{
protected:
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& baseOptions,
                                             const XmlNode& node,
                                             int width,
                                             int height) override;
};
