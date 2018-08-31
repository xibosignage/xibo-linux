#pragma once

#include "IMedia.hpp"

class Media : public IMedia
{
public:
    Media(int id, int width, int height, int duration, Render render, const std::string& uri);

    Media(const Media& other) = delete;
    Media& operator=(const Media& other) = delete;

    void stop() override = 0;
    void start() override = 0;
    void request_handler() override { }
    bool is_running() const override;
    void set_size(int width, int height) override;
    void start_timer() override;

    void attach_audio(std::unique_ptr<IMedia> audio) override;
    sigc::signal<void, Gtk::Widget&, int, int>& handler_requested() override;
    sigc::signal<void>& media_timeout() override;

    int id() const override;
    int width() const override;
    int height() const override;
    int duration() const override;
    Render render() const override;
    const std::string& uri() const override;

private:
    int m_id;
    int m_width;
    int m_height;
    int m_duration;
    Render m_render;
    std::string m_uri;

    sigc::signal<void, Gtk::Widget&, int, int> m_handler_requsted;
    sigc::signal<void> m_media_timeout;
    std::unique_ptr<IMedia> m_audio;
    bool m_started = false;

};
