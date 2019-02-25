#pragma once

struct MediaGeometry
{
    enum class ScaleType
    {
        Scaled,
        Stretch
    } scaleType;

    enum class Align
    {
        Left,
        Center,
        Right
    } align;

    enum class Valign
    {
        Top,
        Middle,
        Bottom
    } valign;
};
