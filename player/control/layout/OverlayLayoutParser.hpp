#pragma once

#include "MainLayoutParser.hpp"

class OverlayLayoutParser : public MainLayoutParser
{
protected:
    std::shared_ptr<Xibo::Image> createBackground(const MainLayoutOptions& /*options*/) override
    {
        return nullptr;
    }
};
