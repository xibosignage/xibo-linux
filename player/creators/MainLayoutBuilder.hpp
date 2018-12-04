#pragma once

#include <memory>
#include <vector>

class IOverlayAdaptor;
class IMainLayout;
class IBackground;
class IRegion;

struct RegionWithPos
{
    std::unique_ptr<IRegion> region;
    int x;
    int y;
};

class MainLayoutBuilder
{
public:
    std::unique_ptr<IMainLayout> build();

    MainLayoutBuilder& width(int width);
    MainLayoutBuilder& height(int height);
    MainLayoutBuilder& background(std::unique_ptr<IBackground>&& background);
    MainLayoutBuilder& regions(std::vector<RegionWithPos>&& regions);

protected:
    virtual std::unique_ptr<IOverlayAdaptor> createAdaptor();

private:
    std::unique_ptr<IMainLayout> createLayout();

    void prepareLayout(IMainLayout& layout);
    void checkWidth(int width);
    void checkHeight(int height);
    void checkRegionsCount(size_t regionsCount);

private:
    int m_width;
    int m_height;
    std::unique_ptr<IBackground> m_background;
    std::vector<RegionWithPos> m_regions;

};
