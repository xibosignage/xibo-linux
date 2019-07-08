#pragma once

#include "BaseLayoutBuilder.hpp"

class OverlayLayoutBuilder : public BaseLayoutBuilder
{
protected:
    std::shared_ptr<IOverlayLayout> createView(const LayoutOptions& options) override;

};
