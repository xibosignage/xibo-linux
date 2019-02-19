#pragma once

#include <memory>
#include <vector>

#include "parsers/LayoutOptions.hpp"
#include "adaptors/IOverlayAdaptor.hpp"

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
    MainLayoutBuilder();
    std::unique_ptr<IMainLayout> build();

    MainLayoutBuilder& options(const ResourcesXlf::LayoutOptions& opts);
    MainLayoutBuilder& background(std::unique_ptr<IBackground>&& background);
    MainLayoutBuilder& regions(std::vector<RegionWithPos>&& regions);
    MainLayoutBuilder& adaptor(std::unique_ptr<IOverlayAdaptor>&& adaptor);

private:
    std::unique_ptr<IMainLayout> createLayout();
    std::unique_ptr<IOverlayAdaptor> createAdaptor();

    int getWidthOption(int width);
    int getHeightOption(int height);

    void prepareLayout(IMainLayout& layout);
    void checkWidth(int width);
    void checkHeight(int height);
    void checkRegionsCount(size_t regionsCount);

private:
    int m_schemeVersion;
    int m_width;
    int m_height;
    std::unique_ptr<IBackground> m_background;
    std::unique_ptr<IOverlayAdaptor> m_adaptor;
    std::vector<RegionWithPos> m_regions;

};
