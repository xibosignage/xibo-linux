#include "StatusScreenController.hpp"

StatusScreenController::StatusScreenController(const std::shared_ptr<StatusScreen>& view) :
    m_view(view)
{

}

void StatusScreenController::onKeyPressed(const Event& ev)
{
    auto keyPress = static_cast<const KeyPressEvent&>(ev);
    if(keyPress.string() == "i")
    {
        m_view->setText(R"(
        General Info:
        Date - yyyy-mm-dd hh:mm:ss
        Version - 1.8
        Code Version - 108
        Content Management System: https://linuxplayer.xibo.co.uk/xmds.php
        Storage Selected: path, percantage occupied
        Display Name: name
        Registered: true/false
        Current Layout: layoutid
        Screen Size: window size
        Memory Limit: mb
        Memory Allocation: mb
        Image Cache Hit Rate:
        Screen Shot Request Interval
        Number of Stats ready to send: 0 - yyyy-mm-dd hh:mm:ss
        Number of Logs ready to send: 0 - yyyy-mm-dd hh:mm:ss

        CMS Status
        Register: registered. Check at yyyy-mm-dd hh:mm:ss
        Schedule: up to date
        Required Files: up to date 56/56
        Queued Network Connections - F: 0 / N: 0. HTTP: 0 / Idle: 0
        Number of Concurrent Failed XMDS Connections: 0

        Schedule Status
        All layouts (*= not scheduled): 49*(D),
        Scheduled Layout: 49,
        Valid Layouts: 49,
        Invalid Layout:
        Next Schedule Update: yyyy-mm-dd hh:mm:ss)");
        m_view->show();
    }
}
