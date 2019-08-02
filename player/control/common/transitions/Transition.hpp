#pragma once

struct Transition
{
    enum class Heading
    {
        In,
        Out
    };

    enum class Type
    {
        Fade,
        Fly
    };

    enum class Direction
    {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
    };
};
