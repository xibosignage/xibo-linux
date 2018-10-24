#pragma once

#include "Media.hpp"
#include "utils/Helpers.hpp"

class IImageAdaptor;

struct ImageProperties
{
    enum class ScaleType
    {
        Scaled,
        Stretch,
        Invalid
    } scaleType;

    enum class Align
    {
        Left,
        Center,
        Right,
        Invalid
    } align;

    enum class Valign
    {
        Top,
        Middle,
        Bottom,
        Invalid
    } valign;
};

class Image : public Media<IVisibleMedia>
{
public:
    Image(int width, int height, const FilePath& path, ImageProperties props, std::unique_ptr<IImageAdaptor>&& handler);

    ImageProperties::ScaleType scaleType() const;
    ImageProperties::Align align() const;
    ImageProperties::Valign valign() const;

    int width() const override;
    int height() const override;
    void scale(double, double) override;

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;

protected:
    void doStop() override;
    void doStart() override;

private:
    std::unique_ptr<IImageAdaptor> m_handler;
    ImageProperties::ScaleType m_scaleType;
    ImageProperties::Align m_align;
    ImageProperties::Valign m_valign;

};
