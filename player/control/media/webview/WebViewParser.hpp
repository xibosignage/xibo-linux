#pragma once

#include "control/media/MediaParser.hpp"

#include "WebViewOptions.hpp"

class FilePath;

class WebViewParser : public MediaParser
{
public:
    WebViewParser(const xml_node& node);    
    WebViewOptions parse();

    Uri uri() override;
    int duration() override;

private:
    std::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);

};
