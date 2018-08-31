#pragma once

#include "IMainWindow.hpp"
#include "IMainLayout.hpp"

class MainWindow : public IMainWindow
{
public:
    MainWindow(int x, int y, bool resizable, bool decorated, bool fullscreen, bool keep_above);

    MainWindow(const MainWindow& other) = delete;
    MainWindow& operator=(const MainWindow& other) = delete;

    void add(std::unique_ptr<IMainLayout> layout) override;
    void show() override;
    IWindowWrapper& handler() override;

private:
    std::unique_ptr<IWindowWrapper> m_handler;
    std::unique_ptr<IMainLayout> m_layout;

};
