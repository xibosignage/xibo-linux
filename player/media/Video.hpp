#pragma once

#include "Media.hpp"

class IVideoHandler;
class FilePath;

class Video : public Media, public IVisible, public IPlayable
{
public:
    Video(int id, int width, int height, const FilePath& path, std::unique_ptr<IVideoHandler>&& handler);

    void play() override;
    void stop() override;

    void scale(double, double) override;
    int width() const override;
    int height() const override;

    void setLooped(bool looped);
    void setMuted(bool muted);

    IWidgetAdaptor& handler() override;
    void apply(MediaVisitor& visitor) override;
    void handleEvent(const Event& ev) override;

protected:
    void show() override { }
    void hide() override { }

private:
    void onVideoFinished();

private:
    std::unique_ptr<IVideoHandler> m_handler;
    bool m_looped = false;

};
