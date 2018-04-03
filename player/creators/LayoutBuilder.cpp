#include "LayoutBuilder.hpp"
#include "RegionBuilder.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"

std::unique_ptr<MainLayout> LayoutBuilder::create(const Params& params)
{
    auto layout = std::make_unique<MainLayout>(params.get<int>("schemaVersion"),
                                               params.get<int>("width"),
                                               params.get<int>("height"),
                                               params.get<std::string>("background"),
                                               params.get<std::string>("bgcolor"));

    auto regions = params.get_child("regions");
    for(auto [region_id, region_params] : regions)
    {
        layout->add_region(RegionBuilder::create(region_params));
    }
    return layout;
}
