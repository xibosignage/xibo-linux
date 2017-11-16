#pragma once

#include "Media.hpp"

class VideoHandler;

class Video : public Media
{
public:
    Video(const std::string& filename, bool looped, bool muted);
    void init(MyRegion* region, const Point& pos, const Size& size, int zindex) override;
    void hide() override;
    void show() override;

private:
    void update_video_size();

private:
    std::string m_filename;
    bool m_looped;
    bool m_muted;
    VideoHandler* m_handler = nullptr;

};
