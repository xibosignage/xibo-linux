#include "ParsedLayoutSchedule.hpp"

bool operator==(const LayoutSchedule& first, const LayoutSchedule& second)
{
    return false;
}

bool operator!=(const LayoutSchedule& first, const LayoutSchedule& second)
{
    return !(first == second);
}
