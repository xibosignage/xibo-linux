#pragma once

#include "Media.hpp"
#include "control/common/Widget.hpp"

class VisibleMediaController
{
public:
    VisibleMediaController(Media& model, const std::shared_ptr<Widget>& mediaView);
};
