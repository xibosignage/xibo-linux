#pragma once

#include "MediaController.hpp"

#include "view/Widget.hpp"

class VisibleMediaController : public MediaController
{
public:
    VisibleMediaController(const std::shared_ptr<MediaModel>& model, const std::shared_ptr<Widget>& mediaView);

protected:
    void doStart() override;
    void doStop() override;

private:
    std::shared_ptr<Widget> m_mediaView;

};
