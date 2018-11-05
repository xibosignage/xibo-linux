#pragma once

#include "IMainLayout.hpp"

#include <vector>

class MainLayout : public IMainLayout
{
public:
    MainLayout(int width, int height, std::unique_ptr<IOverlayAdaptor>&& handler);
    ~MainLayout() override;

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    void scale(double scaleX, double scaleY) override;
    int width() const override;
    int height() const override;

    void setBackground(std::unique_ptr<IBackground>&& background) override;
    void addMediaContainer(std::unique_ptr<IMediaContainer>&& mediaContainer, int x, int y) override;

    IOverlayAdaptor& handler() override;
    void show() override;

private:
    void sortReorderAndShowContainers();
    void sortAndReorderContainers();
    void sortContainersByZorder();
    void removePreviousBackgroundIfSet();
    void scaleContainers(double scaleX, double scaleY);
    void checkContainerSize(int containerWidth, int containerHeight);
    void checkBackgroundSize(int backgroundWidth, int backgroundHeight);

private:
    std::unique_ptr<IOverlayAdaptor> m_handler;
    std::unique_ptr<IBackground> m_background;
    std::vector<std::unique_ptr<IMediaContainer>> m_containers;

};
