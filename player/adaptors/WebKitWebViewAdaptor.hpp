#pragma once

#include "IWebViewAdaptor.hpp"
#include "GtkBaseAdaptor.hpp"

#include <gtkmm/scrolledwindow.h>
#include <webkit/webkit.h>

class WebKitWebViewAdaptor : public GtkBaseAdaptor<IWebViewAdaptor>
{
public:
    WebKitWebViewAdaptor();

    void show() override;
    void hide() override;

    void reload() override;
    void load(const FilePath& page) override;
    void enableTransparency() override;

    void scale(double scaleX, double scaleY) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    Gtk::Widget* get() override;
    void apply(AdaptorVisitor& visitor) override;

private:
    void screenChanged(const Glib::RefPtr<Gdk::Screen>& screen);

private:
    Gtk::ScrolledWindow m_handler;
    WebKitWebView* m_webView = nullptr;

};
