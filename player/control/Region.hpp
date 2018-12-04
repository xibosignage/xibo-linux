#pragma once

#include "IRegion.hpp"
#include <vector>

class IVisible;
class ITimerProvider;

class Region : public IRegion
{
public:
    Region(int id, int width, int height, int zorder, std::unique_ptr<ITimerProvider>&& timer, std::unique_ptr<IFixedLayoutAdaptor>&& handler);
    ~Region() override;

    Region(const Region& other) = delete;
    Region& operator=(const Region& other) = delete;

    int width() const override;
    int height() const override;
    void scale(double scaleX, double scaleY) override;

    void loopMedia() override;
    int id() const override;
    int zorder() const override;
    void show() override;

    void addMedia(std::unique_ptr<IMedia>&& media, int x, int y) override;
    void addMedia(std::unique_ptr<IMedia>&& media) override;
    IFixedLayoutAdaptor& handler() override;

private:
    void initAndAddMediaToList(std::unique_ptr<IMedia>&& media);
    void scaleVisibleMedia(double scaleX, double scaleY);

    void placeMedia(size_t mediaIndex);
    void removeMedia(size_t mediaIndex);
    void onMediaDurationTimeout();

    bool shouldBeMediaReplaced();
    size_t getNextMediaIndex();

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;
    int m_id;
    int m_zorder;
    bool m_mediaLooped = false;

    std::vector<IVisible*> m_visibleMedia;
    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;

};
