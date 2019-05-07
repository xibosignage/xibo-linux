#pragma once

#include "StatusScreen.hpp"
#include "managers/XiboLayoutScheduler.hpp"

class StatusScreenController
{
public:
    StatusScreenController(XiboLayoutScheduler& scheduler, const std::shared_ptr<StatusScreen>& view);

    void onKeyPressed(const std::string& pressedKey);

private:
    std::string collectInfo();

    std::string collectGeneralInfo();
    std::string currentDT();
    std::string collectCmsInfo();

private:
    XiboLayoutScheduler& m_scheduler;
    std::shared_ptr<StatusScreen> m_view;

};
