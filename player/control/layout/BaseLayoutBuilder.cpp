#include "MainLayoutBuilder.hpp"
#include "MainLayout.hpp"
#include "ParsedLayout.hpp"

#include "control/region/RegionBuilder.hpp"
#include "control/region/Region.hpp"

std::unique_ptr<IMainLayout> BaseLayoutBuilder::build(const ParsedLayout& parsedLayout)
{
    auto layoutView = createView(parsedLayout.options);
    auto layout = std::make_unique<MainLayout>(layoutView);

    addRegions(*layout, parsedLayout.regions);

    return layout;
}
void BaseLayoutBuilder::addRegions(IMainLayout& layout, const std::vector<ParsedRegion>& regions)
{
    for(auto&& parsedRegion : regions)
    {
        RegionBuilder regionBuilder;

        auto&& options = parsedRegion.options;
        layout.addRegion(regionBuilder.build(parsedRegion), options.left, options.top, options.zindex);
    }
}
