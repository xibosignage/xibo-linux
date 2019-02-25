#pragma once

#include "Media.hpp"

class IImageAdaptor;
class Uri;

class Image : public Media, public IVisible
{
public:
    MediaGeometry::Align align() const override;
    MediaGeometry::Valign valign() const override;
    MediaGeometry::ScaleType scaleType() const override;

    void show() override;
    void hide() override;

    int width() const override;
    int height() const override;
    void scale(double, double) override;

    IWidgetAdaptor& handler() override;
    void handleEvent(const Event& ev) override;

private:
    friend class ImageBuilder;

    Image(int id, int width, int height, const Uri& uri, MediaGeometry props, std::unique_ptr<IImageAdaptor>&& handler);

    void loadImage(const Uri& uri);

private:
    std::unique_ptr<IImageAdaptor> m_handler;
    MediaGeometry::ScaleType m_scaleType;
    MediaGeometry::Align m_align;
    MediaGeometry::Valign m_valign;

};
