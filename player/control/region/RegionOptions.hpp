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
    Loop loop;
};
