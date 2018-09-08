#pragma once

#include "Media.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>

class WebView : public Media
{
public:
    WebView(int id, int width, int height, int duration, const std::string& uri, int modeId, bool transparent);

    void stop() override;
    void start() override;
    void setSize(int width, int height) override;
    IWidgetAdaptor& handler() override;
    bool transparent() const;

    void apply(MediaVisitor& visitor) override;
private:
    void screenChanged(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    bool m_transparent;
    WebKitWebView* m_webView = nullptr;
    Gtk::ScrolledWindow m_handler;

};
