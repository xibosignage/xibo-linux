#pragma once

#include "VisibleMediaController.hpp"

#include "model/VideoModel.hpp"
#include "view/DrawingArea.hpp"
#include "view/VideoPlayer.hpp"

class VideoController : public VisibleMediaController
{
public:
    VideoController(const std::shared_ptr<VideoModel>& model, const std::shared_ptr<DrawingArea>& videoWindow);

protected:
    void doStart() override;
    void doStop() override;

private:
    std::shared_ptr<VideoModel> m_model;
    VideoPlayer m_player;

};
