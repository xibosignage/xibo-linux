#include "Volume.hpp"

Gst::Volume::Volume() : Gst::Element("volume") {}

Gst::RefPtr<Gst::Volume> Gst::Volume::create()
{
    return Gst::RefPtr<Gst::Volume>(new Gst::Volume);
}

void Gst::Volume::setVolume(double volume)
{
    g_object_set(element(), "volume", volume, nullptr);
}

double Gst::Volume::getVolume() const
{
    double volume;
    g_object_get(element(), "volume", &volume, nullptr);
    return volume;
}
