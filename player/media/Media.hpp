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

    Media(int id, int duration, Render render, const std::string& uri);
    virtual ~Media() = default;

    Media(const Media& other) = delete;
    Media& operator=(const Media& other) = delete;

    virtual void stop() = 0;
    virtual void start() = 0;
    virtual bool is_running() const;
    virtual void set_region(Region* region);
    virtual void set_size(int, int) { }
    virtual void start_timer();
    void attach_audio(std::unique_ptr<Media> audio);
    sigc::signal<void, Gtk::Widget&, Point>& handler_requested();
    sigc::signal<void>& media_timeout();

    const Region& region() const;
    int id() const;
    int duration() const;
    Render render() const;
    const std::string& uri() const;

protected:
    Region* m_region = nullptr;
    int m_id;
    int m_duration;
    Render m_render;
    std::string m_uri;

private:
    sigc::signal<void, Gtk::Widget&, Point> m_handler_requsted;
    sigc::signal<void> m_media_timeout;
    std::unique_ptr<Media> m_audio;
    bool m_started = false;

};
