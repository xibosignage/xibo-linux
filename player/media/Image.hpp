#pragma once

#include "Media.hpp"
#include "ImageProperties.hpp"

class IImageAdaptor;
class FilePath;

class Image : public Media, public IVisible
{
public:
    Image(int id, int width, int height, const FilePath& path, ImageProperties props, std::unique_ptr<IImageAdaptor>&& handler);

    ImageProperties::ScaleType scaleType() const;
    ImageProperties::Align align() const;
    ImageProperties::Valign valign() const;

    void show() override;
    void hide() override;

    int width() const override;
    int height() const override;
    void scale(double, double) override;

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;
    void handleEvent(const Event& ev) override;

private:
    void loadImage(const FilePath& path);

private:
    std::unique_ptr<IImageAdaptor> m_handler;
    ImageProperties::ScaleType m_scaleType;
    ImageProperties::Align m_align;
    ImageProperties::Valign m_valign;

};
