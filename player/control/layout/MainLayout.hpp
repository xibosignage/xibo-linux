#pragma once

#include "control/region/Region.hpp"
#include "control/widgets/Image.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>

using SignalLayoutExpired = boost::signals2::signal<void()>;

namespace Xibo
{
    class MainLayout
    {
    public:
        virtual ~MainLayout() = default;

        virtual void setBackground(std::shared_ptr<Image>&& background) = 0;
        virtual void addRegion(std::unique_ptr<Region>&& region, int x, int y, int z) = 0;
        virtual SignalLayoutExpired& expired() = 0;
        virtual void restart() = 0;
        virtual WidgetPtr view() = 0;
    };
}
