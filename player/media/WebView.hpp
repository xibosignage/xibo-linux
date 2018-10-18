#pragma once

#include "Media.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>

class WebView : public Media<IVisibleMedia>
{
public:
    WebView(int width, int height, int duration, const std::string& uri, bool transparent);

    IWidgetAdaptor& handler() override;
    bool transparent() const;

    void apply(MediaVisitor& visitor) override;

protected:
    void doStop() override;
    void doStart() override;

    void scale(double, double) override { }
    int width() const override { }
    int height() const override { }

private:
    void screenChanged(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    bool m_transparent;
    WebKitWebView* m_webView = nullptr;
    Gtk::ScrolledWindow m_handler;

};
