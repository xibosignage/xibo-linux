#pragma once

#include "StatusScreen.hpp"

class StatusScreenController
{
public:
    StatusScreenController(const std::shared_ptr<StatusScreen>& view);

    void onKeyPressed(const std::string& pressedKey);

private:
    std::shared_ptr<StatusScreen> m_view;

};
