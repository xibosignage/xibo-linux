#pragma once

#include "IMediaContainer.hpp"
#include "utils/ITimerProvider.hpp"

#include <vector>

class MediaContainer : public IMediaContainer
{
public:
    MediaContainer(int width, int height, int zorder, bool looped, std::unique_ptr<ITimerProvider>&& timer, std::unique_ptr<IFixedLayoutAdaptor>&& handler);
    ~MediaContainer() override;

    MediaContainer(const MediaContainer& other) = delete;
    MediaContainer& operator=(const MediaContainer& other) = delete;

    int width() const override;
    int height() const override;
    void scale(double scaleX, double scaleY) override;

    int zorder() const override;
    void show() override;

    void addMedia(std::unique_ptr<IMedia>&& media, int x, int y) override;
    void addMedia(std::unique_ptr<IMedia>&& media) override;
    IFixedLayoutAdaptor& handler() override;

private:
    void initAndAddMediaToList(std::unique_ptr<IMedia>&& media);
    void scaleVisibleMedia(double scaleX, double scaleY);
    void startMedia(size_t mediaIndex);
    void onMediaTimeout();
    size_t getNextMediaIndex();

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;
    std::unique_ptr<ITimerProvider> m_timer;
    int m_zorder;
    bool m_looped = false;

    std::vector<IVisibleMedia*> m_visibleMedia;
    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;

};
