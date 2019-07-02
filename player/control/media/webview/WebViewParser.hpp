#pragma once

#include "control/media/creators/MediaParser.hpp"

class FilePath;

class WebViewParser : public MediaParser
{
protected:
    Uri uri() override;
    int duration() override;
    ExtraOptions parseAdditonalOptions(const xml_node& node) override;

private:
    std::optional<int> parseDuration(const FilePath& path);
    std::string removeEscapedSymbolsFromUri(std::string url);

};
