#pragma once

#include "control/media/MediaParser.hpp"

class FilePath;

class WebViewParser : public MediaParser
{
protected:
    Uri uriFrom(const PtreeNode& node) override;
    int durationFrom(const PtreeNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& options,
                                             const PtreeNode& node,
                                             int width,
                                             int height) override;

private:
    std::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);
};
