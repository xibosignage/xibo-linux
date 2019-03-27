#pragma once

#include "events/EventPublisher.hpp"

#include "model/RegionModel.hpp"
#include "view/RegionView.hpp"
#include "controller/MediaController.hpp"

#include <vector>
#include <boost/noncopyable.hpp>

class RegionController : public EventPublisher<>, private boost::noncopyable
{
public:
    RegionController(const std::shared_ptr<RegionModel>& model, const std::shared_ptr<RegionView>& view);

    void addMedia(std::unique_ptr<MediaController>&& controller);

private:
    void placeMedia(size_t mediaIndex);
    void removeMedia(size_t mediaIndex);
    void onMediaDurationTimeout();

    bool shouldBeMediaReplaced() const;
    size_t getNextMediaIndex() const;
    bool expired() const;

private:
    std::shared_ptr<RegionModel> m_model;
    std::shared_ptr<RegionView> m_view;

    std::vector<std::unique_ptr<MediaController>> m_media;
    size_t m_currentMediaIndex = 0;

};
