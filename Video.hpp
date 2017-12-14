#pragma once

#include "Media.hpp"

class VideoHandler;

class Video : public Media
{
public:
    Video(const Size& size, uint id, uint duration, bool use_duration, const std::string& uri, bool muted, bool looped);

    bool muted() const;
    bool looped() const;

    Gtk::Widget& handler() override;
    void hide() override;
    void show() override;

private:
    bool m_muted;
    bool m_looped;
    VideoHandler* m_handler = nullptr;

};
