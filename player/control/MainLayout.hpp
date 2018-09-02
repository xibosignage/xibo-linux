#pragma once

#include "IMainLayout.hpp"

#include <vector>

class MainLayout : public IMainLayout
{
public:
    MainLayout(int width, int height);
    MainLayout(int width, int height, std::unique_ptr<IOverlayWrapper> handler);

    ~MainLayout() override;

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    void setBackground(std::unique_ptr<IBackground> background) override;

    void addRegion(std::unique_ptr<IRegion> region) override;
    void removeAllRegions() override;
    IRegion& region(size_t index) override;
    size_t regionsCount() const override;

    IOverlayWrapper& handler() override;
    void show() override;

private:
    void reorderRegions();

private:
    std::unique_ptr<IOverlayWrapper> m_handler;
    std::unique_ptr<IBackground> m_background;
    std::vector<std::unique_ptr<IRegion>> m_regions;
};
