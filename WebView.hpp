#pragma once

#include "Media.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>

#include <spdlog/spdlog.h>
#include "constants.hpp"

class WebView : public Media
{
public:
    WebView(const std::string& filename);
    void init(MyRegion* region, const Point& pos, const Size& size, int zindex) override;
    void hide() override;
    void show() override;
    std::string get_filename() const;

private:
    void screen_changed(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    WebKitWebView* m_webView = nullptr;
    Gtk::ScrolledWindow m_handler;
    std::string m_filename;

};
