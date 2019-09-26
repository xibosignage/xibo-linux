#pragma once

#include "control/common/IWidget.hpp"

#include <string>

class IStatusScreen : public IWidget
{
public:
    virtual void setText(const std::string& text) = 0;
};
