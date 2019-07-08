#pragma once

#include "common/uri/Uri.hpp"

#include "../common/transitions/Transition.hpp"
#include "MediaGeometry.hpp"

struct MediaOptions
{
    struct Type{
        std::string type;
        std::string render;
    } type;

    int id;
    Uri uri;
    int duration;
    MediaGeometry geometry;
    boost::optional<Transition> inTrans;
    boost::optional<Transition> outTrans;
};
