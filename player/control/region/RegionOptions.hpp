#pragma once

struct RegionOptions
{
    enum class Loop
    {
        Disable,
        Enable
    };

    int id;
    int width;
    int height;
    int left;
    int top;
    int zindex;
    Loop loop;
};
