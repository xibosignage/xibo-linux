#pragma once

#include "IMediaContainer.hpp"

#include <vector>
#include <sigc++/signal.h>

class MediaContainer : public IMediaContainer
{
public:
    MediaContainer(int id, int zorder, bool looped);
    MediaContainer(int id, int zorder, bool looped, std::unique_ptr<IFixedLayoutAdaptor>);

    ~MediaContainer() override;

    MediaContainer(const MediaContainer& other) = delete;
    MediaContainer& operator=(const MediaContainer& other) = delete;

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    int id() const override;
    int zorder() const override;
    void show() override;

    void addMedia(std::unique_ptr<IMedia> media, int x, int y) override;
    void addMedia(std::unique_ptr<IMedia> media) override;
    IFixedLayoutAdaptor& handler() override;

private:
    void onMediaTimeout();
    void initAndAddMediaToList(std::unique_ptr<IMedia> media);

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;

    int m_id;
    int m_zorder;
    bool m_looped;

    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;
};
