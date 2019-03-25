#include "VisibleMediaController.hpp"

VisibleMediaController::VisibleMediaController(const std::shared_ptr<MediaModel>& model,
                                               const std::shared_ptr<Widget>& mediaView) :
    MediaController(model), m_mediaView(mediaView)
{
}

void VisibleMediaController::doStart()
{ 
    m_mediaView->show();
}

void VisibleMediaController::doStop()
{
    m_mediaView->hide();
}
