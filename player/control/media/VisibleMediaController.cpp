#include "VisibleMediaController.hpp"

VisibleMediaController::VisibleMediaController(Media& model, const std::shared_ptr<Widget>& mediaView)
{
    model.started().connect([mediaView](){
        mediaView->show();
    });

    model.stopped().connect([mediaView](){
        mediaView->hide();
    });
}
