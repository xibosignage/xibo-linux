#pragma once

#include "xmds/Schedule.hpp"
#include "model/PlayerSettings.hpp"

struct CollectionResult
{
    PlayerError error;
    PlayerSettings settings;
    Schedule::Result schedule;
};
