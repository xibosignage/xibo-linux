#include "StatusScreenController.hpp"

#include "utils/Resources.hpp"
#include "config.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std::string_literals;

StatusScreenController::StatusScreenController(XiboLayoutScheduler& scheduler, const std::shared_ptr<StatusScreen>& view) :
    m_scheduler(scheduler),
    m_view(view)
{
}

void StatusScreenController::onKeyPressed(const std::string& pressedKey)
{
    if(pressedKey == "i")
    {
        m_view->setText(collectInfo());
        m_view->show();
    }
}

std::string StatusScreenController::collectInfo()
{
    std::stringstream info;

    info << "General Info:" << std::endl << collectGeneralInfo() << std::endl;
//    info << "CMS Info:" << std::endl << collectCmsInfo() << std::endl;
    info << "Schedule Info:" << std::endl << m_scheduler.status() << std::endl;

    return info.str();
}

std::string StatusScreenController::collectGeneralInfo()
{
    std::stringstream info;

    info << "Date - " << currentDT() << std::endl;
    info << "Version - " << ProjectResources::version() << std::endl;
    info << "Code Version - " << std::to_string(ProjectResources::codeVersion()) << std::endl;
    info << "Content Management System - " << "https://linuxplayer.xibo.co.uk" << std::endl; // settings
    info << "Storage Selected - " << Resources::resDirectory().string() << std::endl;
    info << "Display Name - " << "Display" << std::endl; // settings
    info << "Screen Size: - " << "1920 x 1080" << std::endl; // window
//    info << "Memory Limit - " << "150 MB" << std::endl; // system
    info << "ScreenShot Request Interval - " << "900 seconds" << std::endl; // settings
    info << "Number of Logs ready to send - " << "0" << std::endl; // xmllogsrepo

    return info.str();
}

std::string StatusScreenController::currentDT()
{
    return boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
}

std::string StatusScreenController::collectCmsInfo()
{
    std::stringstream info;

    info << "Registered - " << "true" << std::endl; // cms
    info << "Checked at - " << "yyyy-mm-dd hh:mm:ss" << std::endl; // cms
//    info << "Schedule - " << "up to date" << std::endl;
    info << "Required Files - " << "up to date 56/56" << std::endl;
    info << "Queued Network Connections- " << "F: 0 / N: 0. HTTP: 0 / Idle: 0" << std::endl; // http manager
//    info << "Number of Concurrent Failed XMDS Connections - " << "0" << std::endl;

    return info.str();
}
