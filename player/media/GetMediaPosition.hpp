#pragma once

#include "MediaVisitor.hpp"
#include "ImageProperties.hpp"

class GetMediaPosition : public MediaVisitor
{
public:
    GetMediaPosition(int containerWidth, int containerHeight);

    int getMediaX() const;
    int getMediaY() const;

    void visit(Image& image) override;
    void visit(Audio&) override;
    void visit(Video&) override;
    void visit(WebView&) override;

private:
    int getImageX(ImageProperties::Align align, int imageWidth);
    int getImageY(ImageProperties::Valign valign, int imageHeight);

private:
    int m_mediaX;
    int m_mediaY;
    int m_containerWidth;
    int m_containerHeight;

};
