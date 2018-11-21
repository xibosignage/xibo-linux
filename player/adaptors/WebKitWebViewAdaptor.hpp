#pragma once

#include "IWebViewAdaptor.hpp"
#include "GtkAdaptor.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>

class WebKitWebViewAdaptor : public GtkAdaptor<IWebViewAdaptor>
{
public:
    WebKitWebViewAdaptor();

    void show() override;

    void reload() override;
    void load(const FilePath& page) override;
    void enableTransparency() override;

    Gtk::ScrolledWindow& get() override;

private:
    void screenChanged(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    Gtk::ScrolledWindow m_handler;
    WebKitWebView* m_webView = nullptr;
    sigc::connection m_sizeAllocateConnection;
};
