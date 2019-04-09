#include "VisibleMedia.hpp"

// FIXME MediaOptions to VisibleMediaOptions
VisibleMedia::VisibleMedia(const MediaOptions& options, Widget& view) :
    Media(options),
    m_options(options)
{
    started().connect([&view](){
        view.show();
    });

    stopped().connect([&view](){
        view.hide();
    });
}

MediaGeometry::Align VisibleMedia::align() const
{
    return m_options.geometry.align;
}

MediaGeometry::Valign VisibleMedia::valign() const
{
    return m_options.geometry.valign;
}
