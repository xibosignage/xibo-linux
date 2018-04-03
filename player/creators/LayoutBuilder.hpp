#pragma once

#include "utils/constants.hpp"
#include <memory>

class MainLayout;

class LayoutBuilder
{
public:
    LayoutBuilder() = delete;
    static std::unique_ptr<MainLayout> create(const Params& params);

};
