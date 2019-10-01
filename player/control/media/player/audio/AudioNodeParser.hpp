#pragma once

#include "control/media/MediaParser.hpp"

class AudioNodeParser : public MediaParser
{
protected:
    MediaOptions::Type typeFrom(const PtreeNode& node) override;
    int idFrom(const PtreeNode& node) override;
    Uri uriFrom(const PtreeNode& node) override;
    int durationFrom(const PtreeNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& baseOptions,
                                             const PtreeNode& node,
                                             int width,
                                             int height) override;
};
