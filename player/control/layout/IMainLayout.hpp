#pragma once

#include <boost/signals2/signal.hpp>
#include <memory>

using SignalLayoutExpired = boost::signals2::signal<void()>;

class IRegion;
class IOverlayLayout;

class IMainLayout
{
public:
    virtual ~IMainLayout() = default;

    virtual void addRegion(std::unique_ptr<IRegion>&& region, int x, int y, int z) = 0;
    virtual SignalLayoutExpired& expired() = 0;

    virtual void restart() = 0;

    virtual std::shared_ptr<IOverlayLayout> view() = 0;
};
