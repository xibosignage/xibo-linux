#pragma once

#include "control/media/MediaParser.hpp"

class AudioNodeParser : public MediaParser
{
protected:
    MediaOptions::Type typeFrom(const XmlNode& node) override;
    int idFrom(const XmlNode& node) override;
    Uri uriFrom(const XmlNode& node) override;
    int durationFrom(const XmlNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& baseOptions,
                                             const XmlNode& node,
                                             int width,
                                             int height) override;
};
