#pragma once

#include "IRegion.hpp"
#include "IRegionView.hpp"
#include "RegionOptions.hpp"

#include "control/media/IMedia.hpp"

#include <boost/noncopyable.hpp>

class Region : public IRegion, private boost::noncopyable
{
public:
    Region(int id, RegionOptions::Loop loop, const std::shared_ptr<IRegionView>& view);

    void addMedia(std::unique_ptr<IMedia>&& media, int x, int y) override;
    void start() override;
    SignalRegionExpired& expired() override;

    std::shared_ptr<IRegionView> view() override;

private:
    void placeMedia(size_t mediaIndex);
    void removeMedia(size_t mediaIndex);
    void onMediaDurationTimeout();
    void onMediaRemoved();

    bool shouldBeMediaReplaced() const;
    size_t getNextMediaIndex() const;
    bool isExpired() const;

private:
    int m_id;
    RegionOptions::Loop m_loop;
    std::shared_ptr<IRegionView> m_view;
    std::vector<std::unique_ptr<IMedia>> m_media;
    size_t m_currentMediaIndex = 0;
    SignalRegionExpired m_regionExpired;
};
