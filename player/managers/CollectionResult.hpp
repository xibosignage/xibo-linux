#pragma once

#include "xmds/Schedule.hpp"
#include "control/PlayerSettings.hpp"

struct CollectionResult
{
    boost::system::error_code error;
    PlayerSettings settings;
    Schedule::Result schedule;
};
