#pragma once

#include "IMediaContainer.hpp"
#include <vector>

class MediaContainer : public IMediaContainer
{
public:
    MediaContainer(int zorder, std::unique_ptr<IFixedLayoutAdaptor>&& handler);
    ~MediaContainer() override;

    MediaContainer(const MediaContainer& other) = delete;
    MediaContainer& operator=(const MediaContainer& other) = delete;

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    void loopMediaInContainer() override;
    int zorder() const override;
    void show() override;

    void addMedia(std::unique_ptr<IMedia>&& media, int x, int y) override;
    void addMedia(std::unique_ptr<IMedia>&& media) override;
    void removeAllMedia() override;
    IFixedLayoutAdaptor& handler() override;

private:
    void onMediaTimeout();
    IMedia& initAndAddMediaToList(std::unique_ptr<IMedia>&& media);
    void showCurrentMedia();
    void checkContainerNewSize(int width, int height);

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;
    int m_zorder;

    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;
    bool m_looped = false;

};
