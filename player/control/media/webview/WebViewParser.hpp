#pragma once

#include "control/media/MediaParser.hpp"

class FilePath;

class WebViewParser : public MediaParser
{
protected:
    Uri uriFrom(const xml_node& node) override;
    int durationFrom(const xml_node& node) override;
    ExtraOptions parseExtraOptionsImpl(const xml_node& node) override;

private:
    std::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);

};
