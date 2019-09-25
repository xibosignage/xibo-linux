#pragma once

#include "MainLayoutParser.hpp"

class OverlayLayoutParser : public MainLayoutParser
{
protected:
    std::shared_ptr<IOverlayLayout> createView(const LayoutOptions& options) override;
};
