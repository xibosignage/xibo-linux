#pragma once

#include "control/media/MediaParser.hpp"

class FilePath;

class WebViewParser : public MediaParser
{
protected:
    Uri uriFrom(const XmlNode& node) override;
    int durationFrom(const XmlNode& node) override;
    std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& options,
                                             const XmlNode& node,
                                             int width,
                                             int height) override;

private:
    boost::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);
};
