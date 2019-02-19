#pragma once

#include <memory>
#include <vector>

#include "AbstractBuilder.hpp"

#include "control/IMainLayout.hpp"
#include "parsers/LayoutOptions.hpp"
#include "adaptors/GtkOverlayAdaptor.hpp"

class IBackground;
class IRegion;
class MainLayoutBuilder;

struct RegionWithPos
{
    std::unique_ptr<IRegion> region;
    int x;
    int y;
};

template<>
struct BuilderTraits<MainLayoutBuilder>
{
    using Component = IMainLayout;
    using DefaultHandler = GtkOverlayAdaptor;
    using Options = ResourcesXlf::LayoutOptions;
};

class MainLayoutBuilder : public AbstractBuilder<MainLayoutBuilder>
{
public:
    MainLayoutBuilder();

    MainLayoutBuilder& background(std::unique_ptr<IBackground>&& background);
    MainLayoutBuilder& regions(std::vector<RegionWithPos>&& regions);

protected:
    MainLayoutBuilder& retrieveOptions(const ResourcesXlf::LayoutOptions& opts) override;
    std::unique_ptr<IMainLayout> create() override;
    void doSetup(IMainLayout& layout) override;

private:
    int getWidthOption(int width);
    int getHeightOption(int height);

    void checkWidth(int width);
    void checkHeight(int height);
    void checkRegionsCount(size_t regionsCount);

private:
    int m_schemeVersion;
    int m_width;
    int m_height;
    std::unique_ptr<IBackground> m_background;
    std::vector<RegionWithPos> m_regions;

};
