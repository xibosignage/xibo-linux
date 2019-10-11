#pragma once

#include "control/media/Media.hpp"
#include "control/region/Region.hpp"
#include "control/region/RegionOptions.hpp"
#include "control/widgets/FixedLayout.hpp"

#include <boost/noncopyable.hpp>

class RegionImpl : public Xibo::Region, private boost::noncopyable
{
public:
    RegionImpl(const RegionOptions& options);

    void addMedia(std::unique_ptr<Xibo::Media>&& media) override;
    void start() override;
    SignalRegionExpired& expired() override;
    std::shared_ptr<Xibo::Widget> view() override;

private:
    void placeMedia(size_t mediaIndex);
    void removeMedia(size_t mediaIndex);
    void onMediaDurationTimeout();
    void onMediaRemoved();
    std::pair<int, int> calcMediaPosition(Xibo::Media& media);

    bool shouldBeMediaReplaced() const;
    size_t getNextMediaIndex() const;
    bool isExpired() const;

private:
    RegionOptions options_;
    std::shared_ptr<Xibo::FixedLayout> view_;
    std::vector<std::unique_ptr<Xibo::Media>> media_;
    size_t currentMediaIndex_;
    SignalRegionExpired regionExpired_;
};
