#pragma once

#include <memory>
#include <sigc++/signal.h>

using SignalLayoutExpired = sigc::signal<void()>;

class IRegion;
class IMainLayoutView;

class IMainLayout
{
public:
    virtual ~IMainLayout() = default;

    virtual void addRegion(std::unique_ptr<IRegion>&& region, int x, int y, int z) = 0;
    virtual SignalLayoutExpired expired() = 0;

    virtual std::shared_ptr<IMainLayoutView> view() = 0;

};
