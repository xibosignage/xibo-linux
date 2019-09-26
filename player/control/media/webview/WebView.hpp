#pragma once

#include "IWebView.hpp"
#include "control/common/Widget.hpp"

#include <gtkmm/scrolledwindow.h>

struct _WebKitWebView;
using WebKitWebView = _WebKitWebView;

class Uri;

class WebView : public Widget<IWebView>
{
public:
    WebView(int width, int height);

    void show() override;
    void setSize(int width, int height) override;

    void reload() override;
    void load(const Uri& uri) override;
    void enableTransparency() override;

    Gtk::ScrolledWindow& get() override;

private:
    void screenChanged(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    Gtk::ScrolledWindow m_handler;
    WebKitWebView* m_webView = nullptr;
    sigc::connection m_sizeAllocateConnection;
};
