#pragma once

#include "control/layout/LayoutStat.hpp"
#include "control/media/MediaStat.hpp"
#include "control/region/Region.hpp"
#include "control/widgets/Image.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>

using SignalLayoutExpired = boost::signals2::signal<void()>;
using SignalLayoutStatReady = boost::signals2::signal<void(const LayoutStat&)>;
using SignalLayoutMediaStatsReady = boost::signals2::signal<void(const std::vector<MediaStat>&)>;

namespace Xibo
{
    class MainLayout
    {
    public:
        virtual ~MainLayout() = default;

        virtual void setBackground(std::shared_ptr<Image>&& background) = 0;
        virtual void addRegion(std::unique_ptr<Region>&& region, int x, int y, int z) = 0;
        virtual SignalLayoutExpired& expired() = 0;
        virtual SignalLayoutStatReady& statReady() = 0;
        virtual SignalLayoutMediaStatsReady& mediaStatsReady() = 0;
        virtual void restart() = 0;
        virtual std::shared_ptr<Widget> view() = 0;
        virtual int id() const = 0;
    };
}
