#pragma once

#include "Media.hpp"

#include "control/common/Widget.hpp"

class VisibleMedia : public Media
{
public:
    VisibleMedia(const MediaOptions& options, Widget& view);

    MediaGeometry::Align align() const;
    MediaGeometry::Valign valign() const;

private:
    MediaOptions m_options;

};
