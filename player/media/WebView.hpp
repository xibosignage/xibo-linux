#pragma once

#include "Media.hpp"
#include "options/WebViewOptions.hpp"

class IWebViewAdaptor;
class FilePath;

class WebView : public Media, public IVisible
{
public:
    void show() override;
    void hide() override;

    void scale(double, double) override;
    int width() const override;
    int height() const override;

    IWidgetAdaptor& handler() override;
    void handleEvent(const Event& ev) override;

    MediaGeometry::Align align() const override;
    MediaGeometry::Valign valign() const override;
    MediaGeometry::ScaleType scaleType() const override;

    void setTransparent(WebViewOptions::Transparency transparency);

private:
    friend class WebViewBuilder;

    WebView(int id, int width, int height, const FilePath& path, std::unique_ptr<IWebViewAdaptor>&& handler);

private:
    std::unique_ptr<IWebViewAdaptor> m_handler;

};
