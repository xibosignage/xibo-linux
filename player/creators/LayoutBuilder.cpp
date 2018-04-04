#include "LayoutBuilder.hpp"
#include "RegionBuilder.hpp"

#include "parsers/LayoutParser.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"

std::unique_ptr<MainLayout> LayoutBuilder::create()
{
    LayoutParser parser;
    auto params = parser.parse_layout();
    auto layout = std::make_unique<MainLayout>(params.get<int>("schemaVersion"),
                                               params.get<int>("width"),
                                               params.get<int>("height"),
                                               params.get<std::string>("background"),
                                               params.get<std::string>("bgcolor"));

    auto regions = parser.regions_ids();
    for(auto region_id : regions)
    {
        spdlog::get(LOGGER)->debug("here");
        layout->add_region(RegionBuilder::create(region_id));
    }
    return layout;
}
