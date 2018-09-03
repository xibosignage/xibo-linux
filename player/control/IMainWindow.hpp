#pragma once

#include <memory>

class IMainLayout;
class IWindowAdaptor;

class IMainWindow
{
public:
    virtual ~IMainWindow() = default;
    virtual void add(std::unique_ptr<IMainLayout> layout) = 0;
    virtual void show() = 0;
    virtual IWindowAdaptor& handler() = 0;
};
