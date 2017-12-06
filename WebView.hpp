#pragma once

#include "Media.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>
#include <spdlog/spdlog.h>

#include "constants.hpp"

class WebView : public Media
{
public:
    WebView(int id, int duration, const std::string& uri, int modeId, bool transparent);

    void hide() override;
    void show() override;

private:
    void screen_changed(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    bool m_transparent;

    WebKitWebView* m_webView = nullptr;
    Gtk::ScrolledWindow m_handler;

};
