#pragma once

#include "xmds/Schedule.hpp"
#include "control/PlayerSettings.hpp"

struct CollectionResult
{
    bool success;
    PlayerSettings settings;
    Schedule::Result schedule;
};
