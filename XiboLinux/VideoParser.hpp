#pragma once

#include "MediaParser.hpp"

using ParsedVideo = std::tuple<uint, uint, bool, std::string, bool, bool>;

class VideoParser : public MediaParser<ParsedVideo>
{
public:
    using MediaParser<ParsedVideo>::MediaParser;
    ParsedVideo parse();

};
