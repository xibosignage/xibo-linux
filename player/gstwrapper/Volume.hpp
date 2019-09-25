#pragma once

#include "Element.hpp"

namespace Gst
{
class Volume : public Gst::Element
{
public:
    static Gst::RefPtr<Gst::Volume> create();
    void setVolume(double volume);
    double getVolume() const;

private:
    Volume();
};
}
