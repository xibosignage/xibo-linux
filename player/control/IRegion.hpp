#pragma once

#include <memory>

#include "utils/IObservable.hpp"

class IRegionContent;
class IFixedLayoutAdaptor;

class IRegion : public IObservable
{
public:
    virtual ~IRegion() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void scale(double scaleX, double scaleY) = 0;

    virtual void loopContent() = 0;
    virtual bool contentLooped() const = 0;
    virtual int id() const = 0;
    virtual int zorder() const = 0;
    virtual void show() = 0;

    virtual void addContent(std::unique_ptr<IRegionContent>&& media, int x, int y) = 0;
    virtual IFixedLayoutAdaptor& handler() = 0;

};
