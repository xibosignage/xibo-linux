#pragma once

#include "Media.hpp"

class IImageAdaptor;
class FilePath;

class Image : public Media, public IVisible
{
public:
    Image(int id, int width, int height, const FilePath& path, MediaGeometry props, std::unique_ptr<IImageAdaptor>&& handler);

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
    void loadImage(const FilePath& path);

private:
    std::unique_ptr<IImageAdaptor> m_handler;
    MediaGeometry::ScaleType m_scaleType;
    MediaGeometry::Align m_align;
    MediaGeometry::Valign m_valign;

};
