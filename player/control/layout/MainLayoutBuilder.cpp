#include "MainLayoutBuilder.hpp"
#include "MainLayout.hpp"
#include "MainLayoutView.hpp"
#include "ParsedLayout.hpp"

#include "control/region/RegionBuilder.hpp"
#include "control/region/Region.hpp"

#include "common/FilePath.hpp"
#include "control/common/Image.hpp"

std::unique_ptr<IMainLayout> MainLayoutBuilder::build(const ParsedLayout& parsedLayout)
{
    auto layoutView = createView(parsedLayout.options);
    auto layout = std::make_unique<MainLayout>(layoutView);

    addRegions(*layout, parsedLayout.regions);

    return layout;
}

std::shared_ptr<IMainLayoutView> MainLayoutBuilder::createView(const LayoutOptions& options)
{
    auto layoutView = std::make_shared<MainLayoutView>(options.width, options.height);

    layoutView->addBackground(createBackground(options));

    return layoutView;
}

std::shared_ptr<IImage> MainLayoutBuilder::createBackground(const LayoutOptions& options)
{
    auto background = std::make_shared<Image>(options.width, options.height);

    if(options.backgroundUri.isValid())
        background->loadFromFile(options.backgroundUri.path(), false);
    else
        background->setColor(options.backgroundColor);

    return background;
}

void MainLayoutBuilder::addRegions(IMainLayout& layout, const std::vector<ParsedRegion>& regions)
{
    for(auto&& parsedRegion : regions)
    {
        RegionBuilder regionBuilder;

        auto&& options = parsedRegion.options;
        layout.addRegion(regionBuilder.build(parsedRegion), options.left, options.top, options.zindex);
    }
}
