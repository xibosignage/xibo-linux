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

    Media(const Region& region, int id, int duration, Render render, const std::string& uri);
    virtual ~Media() = default;

    Media(const Media& other) = delete;
    Media& operator=(const Media& other) = delete;

    virtual void stop() = 0;
    virtual void start() = 0;
    virtual bool is_running() const;
    sigc::signal<void, Gtk::Widget&, int, int>& handler_requested();

    const Region& region() const;
    int id() const;
    int duration() const;
    Render render() const;
    const std::string& uri() const;

protected:
    const Region& m_region;
    int m_id;
    int m_duration;
    Render m_render;
    std::string m_uri;

private:
    sigc::signal<void, Gtk::Widget&, int, int> m_handler_requsted;
    bool m_started = false;

};
