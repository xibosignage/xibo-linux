#pragma once

#include "control/region/Region.hpp"
#include "control/widgets/Image.hpp"
#include "stat/PlayingStat.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>

using SignalLayoutExpired = boost::signals2::signal<void()>;
using SignalLayoutStatReady = boost::signals2::signal<void(const PlayingStat&)>;
using MediaPlayingStats = std::multimap<int, PlayingStat>;
using SignalLayoutMediaStatsReady = boost::signals2::signal<void(const MediaPlayingStats&)>;

namespace Xibo
{
    class MainLayout
    {
    public:
        virtual ~MainLayout() = default;

        virtual void setBackground(std::shared_ptr<Image>&& background) = 0;
        virtual void addRegion(std::unique_ptr<Region>&& region, int left, int top, int zorder) = 0;
        virtual SignalLayoutExpired& expired() = 0;
        virtual SignalLayoutStatReady& statReady() = 0;
        virtual SignalLayoutMediaStatsReady& mediaStatsReady() = 0;
        virtual void restart() = 0;
        virtual std::shared_ptr<Widget> view() = 0;
        virtual int id() const = 0;
    };
}
