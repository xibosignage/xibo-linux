#pragma once

#include "xmds/Schedule.hpp"
#include "control/PlayerSettings.hpp"

struct CollectionResult
{
    PlayerError error;
    PlayerSettings settings;
    Schedule::Result schedule;
};
