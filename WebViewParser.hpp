#pragma once

#include "MediaParser.hpp"

using ParsedWebView = std::tuple<uint, uint, bool, std::string, int, bool>;

class WebViewParser : public MediaParser<ParsedWebView>
{
public:
    using MediaParser<ParsedWebView>::MediaParser;
    ParsedWebView parse();

};
