#pragma once

#include "model/MediaGeometry.hpp"
#include "utils/Uri.hpp"

#include <boost/optional/optional.hpp>

struct LayoutOptions
{
    int width;
    int height;
    boost::optional<std::string> backgroundUri;
    std::string backgroundColor;
};

struct RegionOptions
{
    int id;
    int width;
    int height;
    int left;
    int top;
    int zindex;
    bool loop;
};

struct MediaOptions
{
    int id;
    Uri uri;
    int duration;
};

struct ImageOptions : public MediaOptions
{
    MediaGeometry::ScaleType scaleType;
    MediaGeometry::Align align;
    MediaGeometry::Valign valign;
};

struct VideoOptions : public MediaOptions
{
    bool muted;
    bool looped;
};

struct AudioOptions : public MediaOptions
{
    bool muted;
    bool looped;
    int volume;
};

struct WebViewOptions : public MediaOptions
{
    bool transparency;
    int m_mode;
};
