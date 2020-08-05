#pragma once

#include "control/widgets/Widget.hpp"

#include <boost/signals2/signal.hpp>
#include <string>

using ExitWithoutRestartRequested = boost::signals2::signal<void()>;

namespace Xibo
{
    class StatusScreen : public Widget
    {
    public:
        virtual void setMainInfo(const std::string& text) = 0;
        virtual void setInvalidFiles(const std::vector<std::string>& files) = 0;
        virtual ExitWithoutRestartRequested& exitWithoutRestartRequested() = 0;
    };
}
