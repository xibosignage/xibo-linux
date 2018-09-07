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
    bool isRunning() const override;
    void setSize(int width, int height) override;
    void startTimer() override;

    void attachAudio(std::unique_ptr<IMedia> audio) override;
    void connect(OnMediaTimeout callback) override;

    int id() const override;
    int width() const override;
    int height() const override;
    int duration() const override;
    Render render() const override;
    const std::string& uri() const override;

protected:
    sigc::signal<void>& mediaTimeout();

private:
    int m_id;
    int m_width;
    int m_height;
    int m_duration;
    Render m_render;
    std::string m_uri;

    sigc::signal<void> m_mediaTimeout;
    std::unique_ptr<IMedia> m_audio;
    bool m_started = false;

};
