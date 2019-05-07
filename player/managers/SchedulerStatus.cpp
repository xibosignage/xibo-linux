#include "SchedulerStatus.hpp"

#include <sstream>

std::string layoutsToString(const std::vector<int>& layouts)
{
    std::stringstream result;

    for(int id : layouts)
    {
        result << std::to_string(id) << " ";
    }

    return result.str();
}

std::ostream& operator <<(std::ostream& out, const SchedulerStatus& status)
{
    out << "Last Update - " << status.generatedTime << std::endl;
    out << "Current Layout - " << status.currentLayout << std::endl;
//    out << "All layouts (*= not scheduled) - " << "49*(D)" << std::endl;
    out << "Scheduled Layouts - " << layoutsToString(status.scheduledLayouts) << std::endl;
    out << "Valid Layouts - " << layoutsToString(status.validLayouts) << std::endl;
    out << "Invalid Layouts - " << layoutsToString(status.invalidLayouts) << std::endl;
//    out << "Next Schedule Update - " << "yyyy-mm-dd hh:mm:ss" << std::endl;

    return out;
}
