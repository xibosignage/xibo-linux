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
    void set_size(int width, int height) override;
    void request_handler() override;
    bool transparent() const;

private:
    void screen_changed(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    bool m_transparent;
    WebKitWebView* m_web_view = nullptr;
    Gtk::ScrolledWindow m_handler;

};
