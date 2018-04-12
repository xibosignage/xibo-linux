#include "LayoutBuilder.hpp"
#include "RegionBuilder.hpp"

#include "parsers/LayoutParser.hpp"
#include "parsers/ParserHelpers.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"

std::unique_ptr<MainLayout> LayoutBuilder::create(const ParsedLayout& object)
{
    auto layout = create_from_params(object);

    for(auto region : object.regions)
    {
        layout->add_region(RegionBuilder::create(region));
    }
    return layout;
}

std::unique_ptr<MainLayout> LayoutBuilder::create_from_params(const ParsedLayout& object)
{
    std::string bgimage = object.bgimage.value_or(std::string{});
    std::string bgcolor = object.bgcolor.value_or(std::string{});

    return std::make_unique<MainLayout>(object.schemaVersion, object.width, object.height, bgimage, bgcolor);
}
