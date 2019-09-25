#pragma once

#include "control/media/MediaParser.hpp"

class FilePath;

class WebViewParser : public MediaParser
{
protected:
    Uri uriFrom(const ptree_node& node) override;
    int durationFrom(const ptree_node& node) override;
    ExtraOptions extraOptionsImpl(const ptree_node& node) override;

private:
    std::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);
};
