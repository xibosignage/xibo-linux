#include "RegionController.hpp"

#include "AddMediaVisitor.hpp"

const int FIRST_CONTENT_INDEX = 0;

RegionController::RegionController(const std::shared_ptr<RegionModel>& model,
                                   const std::shared_ptr<FixedLayout>& view) :
    m_model(model), m_view(view)
{
    for(auto&& mediaModel : m_model->media())
    {
        AddMediaVisitor visitor(*m_view, *this);
        mediaModel->apply(visitor);
    }

    m_view->subscribe(EventType::WidgetShown, [this](const Event&){
        placeMedia(FIRST_CONTENT_INDEX);
    });
}

void RegionController::addMedia(std::unique_ptr<MediaController>&& controller)
{
    controller->subscribe(EventType::DurationExpired, std::bind(&RegionController::onMediaDurationTimeout, this));
    m_media.emplace_back(std::move(controller));
}

void RegionController::placeMedia(size_t mediaIndex)
{
    m_currentMediaIndex = mediaIndex;
    m_media[mediaIndex]->start();
}

void RegionController::removeMedia(size_t mediaIndex)
{
    m_media[mediaIndex]->stop();
}

void RegionController::onMediaDurationTimeout()
{
    if(expired())
    {
        pushEvent(RegionDurationExpiredEvent{m_model->id()});
    }

    if(shouldBeMediaReplaced())
    {
        removeMedia(m_currentMediaIndex);
        placeMedia(getNextMediaIndex());
    }
}

bool RegionController::expired() const
{
    return getNextMediaIndex() == FIRST_CONTENT_INDEX;
}

bool RegionController::shouldBeMediaReplaced() const
{
    return m_media.size() > 1 || m_model->looped() == RegionModel::Loop::Enable;
}

size_t RegionController::getNextMediaIndex() const
{
    size_t nextContentIndex = m_currentMediaIndex + 1;

    if(nextContentIndex >= m_media.size())
        return FIRST_CONTENT_INDEX;

    return nextContentIndex;
}
