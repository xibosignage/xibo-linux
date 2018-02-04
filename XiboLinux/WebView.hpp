#pragma once

#include "Media.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>
#include <spdlog/spdlog.h>

#include "constants.hpp"

class WebView : public Media
{
public:
    WebView(const Size& size, uint id, uint duration, bool use_duration, const std::string& uri, int modeId, bool transparent);

    void hide() override;
    void show() override;
    Gtk::Widget& handler() override;

    bool transparent() const;

private:
    void screen_changed(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    bool m_transparent;

    WebKitWebView* m_web_view = nullptr;
    Gtk::ScrolledWindow m_handler;

};