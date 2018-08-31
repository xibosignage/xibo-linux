#pragma once

#include <spdlog/spdlog.h>
#include <vector>

#include "IMainLayout.hpp"

class MainLayout : public IMainLayout
{
public:
    MainLayout(int width, int height);
    MainLayout(int width, int height, std::shared_ptr<IOverlayWrapper> handler);

    ~MainLayout() override;

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    void set_size(int width, int height) override;
    int width() const override;
    int height() const override;

    void set_background(std::unique_ptr<IBackground> background) override;
    IBackground& background() override;

    void add_region(std::unique_ptr<IRegion> region) override;
    IRegion& region(size_t index) override;
    size_t regions_count() const override;

    IOverlayWrapper& handler() override;
    void show() override;

private:
    void reorder_regions();

private:
    std::shared_ptr<IOverlayWrapper> m_handler;
    std::unique_ptr<IBackground> m_background;
    std::vector<std::unique_ptr<IRegion>> m_regions;
};
