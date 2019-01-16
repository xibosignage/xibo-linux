#pragma once

#include "xmds/Schedule.hpp"
#include "control/PlayerSettings.hpp"

struct CollectionResult
{
    struct Error
    {
        std::string message;

        operator bool() const
        {
            return !message.empty();
        }

    } error;

    PlayerSettings settings;
    Schedule::Result schedule;
};
