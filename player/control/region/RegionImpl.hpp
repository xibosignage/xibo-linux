#pragma once

#include "control/media/Media.hpp"
#include "control/region/Region.hpp"
#include "control/region/RegionOptions.hpp"
#include "control/widgets/FixedContainer.hpp"

#include <boost/noncopyable.hpp>

class RegionImpl : public Xibo::Region, private boost::noncopyable
{
    static constexpr const int MediaOrder = 0;
    static constexpr const int FirstMediaIndex = 0;

public:
    RegionImpl(const RegionOptions& options);

    void addMedia(std::unique_ptr<Xibo::Media>&& media) override;
    void start() override;
    void stop() override;
    SignalRegionExpired& expired() override;
    std::shared_ptr<Xibo::Widget> view() override;

    const MediaList& mediaList() const override;

private:
    void placeMedia(size_t mediaIndex);
    void removeMedia(size_t mediaIndex);
    void onMediaDurationTimeout();

    std::pair<int, int> calcMediaPosition(Xibo::Media& media);

    bool shouldBeMediaReplaced() const;
    size_t getNextMediaIndex() const;
    bool isExpired() const;

private:
    RegionOptions options_;
    std::shared_ptr<Xibo::FixedContainer> view_;
    MediaList mediaList_;
    size_t currentMediaIndex_;
    SignalRegionExpired regionExpired_;
};
