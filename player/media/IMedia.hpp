#pragma once

#include <string>
#include <memory>
#include <sigc++/signal.h>

#include <gtkmm/widget.h> // FIXME

class IMedia
{
public:
    enum class Render
    {
        HTML,
        Native
    };

    virtual ~IMedia() = default;

    virtual void stop() = 0;
    virtual void start() = 0;
    virtual void request_handler() = 0;
    virtual bool is_running() const = 0;
    virtual void set_size(int width, int height) = 0;
    virtual void start_timer() = 0;

    virtual void attach_audio(std::unique_ptr<IMedia> audio) = 0;
    virtual sigc::signal<void, Gtk::Widget&, int, int>& handler_requested() = 0;
    virtual sigc::signal<void>& media_timeout() = 0;

    virtual int id() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual int duration() const = 0;
    virtual Render render() const = 0;
    virtual const std::string& uri() const = 0;

};
