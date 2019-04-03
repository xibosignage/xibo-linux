#pragma once

#include "control/media/Media.hpp"
#include "RegionOptions.hpp"

#include <vector>
#include <boost/noncopyable.hpp>
#include <sigc++/signal.h>

using SignalRegionExpired = sigc::signal<void(int)>;

class Region : private boost::noncopyable
{
public:
    Region(int id, RegionOptions::Loop looped);

    void addMedia(std::unique_ptr<Media>&& media);
    void start();
    SignalRegionExpired expired();

private:
    void placeMedia(size_t mediaIndex);
    void removeMedia(size_t mediaIndex);
    void onMediaDurationTimeout();

    bool shouldBeMediaReplaced() const;
    size_t getNextMediaIndex() const;
    bool isExpired() const;

private:
    int m_id;
    RegionOptions::Loop m_looped;
    std::vector<std::unique_ptr<Media>> m_media;
    size_t m_currentMediaIndex = 0;
    SignalRegionExpired m_regionRexpired;

};
