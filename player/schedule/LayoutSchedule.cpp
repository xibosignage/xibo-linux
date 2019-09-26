#include "LayoutSchedule.hpp"

#include "common/Utils.hpp"

bool operator==(const LayoutSchedule& first, const LayoutSchedule& second)
{
    if (first.generatedTime != second.generatedTime) return false;
    if (first.defaultLayout != second.defaultLayout) return false;

    if (!Utils::containersEqual(first.regularLayouts, second.regularLayouts)) return false;
    if (!Utils::containersEqual(first.overlayLayouts, second.overlayLayouts)) return false;
    if (!Utils::containersEqual(first.globalDependants, second.globalDependants)) return false;

    return true;
}

bool operator!=(const LayoutSchedule& first, const LayoutSchedule& second)
{
    return !(first == second);
}
