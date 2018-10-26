#pragma once

struct ImageProperties
{
    enum class ScaleType
    {
        Scaled,
        Stretch,
        Invalid
    } scaleType;

    enum class Align
    {
        Left,
        Center,
        Right,
        Invalid
    } align;

    enum class Valign
    {
        Top,
        Middle,
        Bottom,
        Invalid
    } valign;
};
