#pragma once

#include "control/common/IWidget.hpp"
#include <memory>

class XiboVideoFrame;

class IVideoWindow : public IWidget
{
public:
    virtual void drawFrame(const std::shared_ptr<XiboVideoFrame>& frame) = 0;
};
