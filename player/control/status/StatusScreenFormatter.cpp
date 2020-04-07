#include "StatusScreenFormatter.hpp"

#include "common/dt/DateTime.hpp"
#include "control/status/StatusInfo.hpp"

std::string StatusScreenFormatter::formatInfo(const StatusInfo& info)
{
    std::stringstream out;

    out << "General Info:" << std::endl << formatGeneralInfo(info.general) << std::endl;
    out << "CMS Info:" << std::endl << formatCmsInfo(info.cms) << std::endl;
    out << "Schedule Info:" << std::endl << formatSchedulerInfo(info.scheduler) << std::endl;
    out << "XMR Info:" << std::endl << formatXmrInfo(info.xmr) << std::endl;

    return out.str();
}

std::string StatusScreenFormatter::formatGeneralInfo(const GeneralInfo& info)
{
    std::stringstream out;

    out << "Date - " << info.currentDateTime.string() << std::endl;
    out << "Version - " << info.projectVersion << std::endl;
    out << "Code Version - " << info.codeVersion << std::endl;
    out << "Content Management System - " << info.cmsAddress << std::endl;
    out << "Storage Selected - " << info.resourcesPath << std::endl;
    out << "Display Name - " << info.displayName << std::endl;
    out << "Screen Size: - " << info.windowWidth << " x " << info.windowHeight << std::endl;
    //    info << "Memory Limit - " << "150 MB" << std::endl;
    out << "ScreenShot Request Interval - " << info.screenShotInterval << " minutes" << std::endl;
    //    out << "Number of Logs ready to send - " << info.numberOfLogsToSend << std::endl;

    return out.str();
}

std::string StatusScreenFormatter::formatCmsInfo(const CmsStatus& info)
{
    std::stringstream out;

    out << "Registered - " << std::boolalpha << info.registered << std::endl;  // cms
    out << "Checked at - " << info.lastChecked.string() << std::endl;          // cms
    //    info << "Schedule - " << "up to date" << std::endl;
    out << "Required Files - " << info.requiredFiles << std::endl;
    //    out << "Queued Network Connections- " << "F: 0 / N: 0. HTTP: 0 / Idle: 0" << std::endl; // http manager
    //    info << "Number of Concurrent Failed XMDS Connections - " << "0" << std::endl;

    return out.str();
}

std::string StatusScreenFormatter::formatSchedulerInfo(const SchedulerStatus& info)
{
    std::stringstream out;

    out << "Last Update - " << info.generatedTime << std::endl;
    out << "Current Layout - " << info.currentLayout << std::endl;
    //    out << "All layouts (*= not scheduled) - " << "49*(D)" << std::endl;
    out << "Scheduled Layouts - " << layoutsToString(info.scheduledLayouts) << std::endl;
    out << "Valid Layouts - " << layoutsToString(info.validLayouts) << std::endl;
    out << "Invalid Layouts - " << layoutsToString(info.invalidLayouts) << std::endl;
    //    out << "Next Schedule Update - " << "yyyy-mm-dd hh:mm:ss" << std::endl;

    return out.str();
}

std::string StatusScreenFormatter::layoutsToString(const std::vector<int>& layouts)
{
    std::stringstream out;

    for (int id : layouts)
    {
        out << std::to_string(id) << " ";
    }

    return out.str();
}

std::string StatusScreenFormatter::formatXmrInfo(const XmrStatus& info)
{
    std::stringstream out;

    out << "Host - " << info.host << std::endl;
    out << "Last heartbeat - " << info.lastHeartbeatDt.string() << std::endl;
    out << "Last message - " << info.lastMessageDt.string() << std::endl;

    return out.str();
}
