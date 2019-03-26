#pragma once

#include "view/StatusScreen.hpp"

class StatusScreenController
{
public:
    StatusScreenController(const std::shared_ptr<StatusScreen>& view);

private:
    std::shared_ptr<StatusScreen> m_view;

};
