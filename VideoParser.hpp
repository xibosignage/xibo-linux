#pragma once

#include "MediaParser.hpp"

struct ParsedVideo : ParsedMedia
{
    ParsedVideo(int _id, int _duration, const std::string& _uri, bool _muted, bool _looped) :
        ParsedMedia{_id, _duration, _uri}, muted(_muted), looped(_looped) { }

    bool muted;
    bool looped;
};

class VideoParser : public MediaParser<ParsedVideo>
{
public:
    using MediaParser<ParsedVideo>::MediaParser;
    ParsedVideo parse();

};
