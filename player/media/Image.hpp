#pragma once

#include "Media.hpp"
#include "utils/Helpers.hpp"

#include <filesystem>

class IImageAdaptor;

class Image : public Media
{
public:
    enum class ScaleType
    {
        Scaled,
        Stretch,
        Invalid
    };

    enum class Align
    {
        Left,
        Center,
        Right,
        Invalid
    };

    enum class Valign
    {
        Top,
        Middle,
        Bottom,
        Invalid
    };

    Image(ScaleType type, Align align, Valign valign, std::unique_ptr<IImageAdaptor>&& handler);

    ScaleType scaleType() const;
    Align align() const;
    Valign valign() const;

    int width() const override;
    int height() const override;
    void setSize(int, int) override { }

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;

protected:
    void doStop() override;
    void doStart() override;

private:
    std::unique_ptr<IImageAdaptor> m_handler;
    ScaleType m_scaleType;
    Align m_align;
    Valign m_valign;

};
