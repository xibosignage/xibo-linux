#pragma once

#include "Media.hpp"

class IWebViewAdaptor;
class Uri;

class WebView : public Media, public IVisible
{
public:
    WebView(int id, int width, int height, const Uri& uri, std::unique_ptr<IWebViewAdaptor>&& handler);

    void show() override;
    void hide() override;

    void scale(double, double) override;
    int width() const override;
    int height() const override;

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;
    void handleEvent(const Event& ev) override;

    void setTransparent(bool transparent);

private:
    std::unique_ptr<IWebViewAdaptor> m_handler;

};
