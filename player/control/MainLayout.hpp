#pragma once

#include "IMainLayout.hpp"

#include <vector>

class MainLayout : public IMainLayout
{
public:
    MainLayout(int width, int height);
    MainLayout(int width, int height, std::unique_ptr<IOverlayAdaptor> handler);

    ~MainLayout() override;

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    void setBackground(std::unique_ptr<IBackground> background) override;
    IBackground& background() override;

    void addMediaContainer(std::unique_ptr<IMediaContainer> mediaContainer, int x, int y) override;
    void removeAllContainers() override;

    IOverlayAdaptor& handler() override;
    void show() override;

private:
    void sortAndReorderContainers();
    void sortContainersByZorder();
    void showBackground();
    void sortReorderAndShowContainers();
    void removePreviousBackground();
    void setBackgroundSize(int width, int height);

private:
    std::unique_ptr<IOverlayAdaptor> m_handler;
    std::unique_ptr<IBackground> m_background;
    std::vector<std::unique_ptr<IMediaContainer>> m_containers;
};
