#pragma once

#include <string>
#include <gtkmm/widget.h>

#include "constants.hpp"

class Region;

class Media
{
public:

    enum class Render
    {
        HTML,
        Native
    };

    Media(int id, int duration, bool use_duration, Render render, const std::string& uri);
    virtual ~Media() = default;

    Media(const Media& other) = delete;
    Media& operator=(const Media& other) = delete;

    virtual void hide() = 0;
    virtual void show() = 0;
    virtual Gtk::Widget& handler() = 0;
    virtual bool is_visible() const;

    int id() const;
    int duration() const;
    bool use_duration() const;
    Render render() const;
    const std::string& uri() const;

    sigc::signal<void>& media_timeout();

protected:
    int m_id;
    int m_duration;
    bool m_use_duration;
    Render m_render;
    std::string m_uri;
    bool m_visible = false;

private:
    sigc::signal<void> m_media_timeout;

};
