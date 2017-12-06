#pragma once

#include "Media.hpp"

class VideoHandler;

class Video : public Media
{
public:
    Video(int id, int duration, const std::string& uri, bool muted, bool looped);

    bool muted() const;
    bool looped() const;

    void set_size(const Size& size) override;
    Gtk::Widget& handler() override;
    void hide() override;
    void show() override;

private:
    bool m_muted;
    bool m_looped;
    VideoHandler* m_handler = nullptr;

};
