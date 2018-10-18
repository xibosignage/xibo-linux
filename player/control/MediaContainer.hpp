#pragma once

#include "IMediaContainer.hpp"

#include <vector>

class MediaContainer : public IMediaContainer
{
public:
    MediaContainer(int zorder, bool looped, std::unique_ptr<IFixedLayoutAdaptor>&& handler);
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
    void removeAllMedia() override;
    IFixedLayoutAdaptor& handler() override;

private:
    void initAndAddMediaToList(std::unique_ptr<IMedia>&& media);
    void showCurrentMedia();
    void checkContainerSize(int width, int height);
    void onMediaTimeout();
    void scaleMedia(double scaleX, double scaleY);

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;
    int m_zorder;
    bool m_looped;

    std::vector<IVisibleMedia*> m_visibleMedia;
    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;

};
