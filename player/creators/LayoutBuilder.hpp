#pragma once

#include <memory>

class MainLayout;

class LayoutBuilder
{
public:
    LayoutBuilder() = delete;
    static std::unique_ptr<MainLayout> create();

};
