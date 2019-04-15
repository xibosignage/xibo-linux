#pragma once

#include "control/common/Widget.hpp"
#include "control/media/VisibleMedia.hpp"

class MediaFactory
{
public:
    virtual ~MediaFactory() = default;
    virtual std::unique_ptr<Media> createModel() = 0;
};

struct ViewInfo
{
    std::shared_ptr<Widget> view;
    MediaGeometry::Align align;
    MediaGeometry::Valign valign;
};

class VisibleMediaFactory
{
public:
    virtual ~VisibleMediaFactory() = default;
    virtual std::unique_ptr<Media> createModel(const std::shared_ptr<Widget>& view) = 0;
    virtual ViewInfo createView(int width, int height) = 0;
};
