#pragma once

#include <memory>

class IMainLayout;
class IWindowWrapper;

class IMainWindow
{
public:
    virtual ~IMainWindow() = default;
    virtual void add(std::unique_ptr<IMainLayout> layout) = 0;
    virtual void show() = 0;
    virtual IWindowWrapper& handler() = 0;
};
