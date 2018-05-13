#include "LayoutBuilder.hpp"
#include "RegionBuilder.hpp"

#include "parsers/LayoutParser.hpp"
#include "parsers/ParserHelpers.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "control/Background.hpp"
#include "utils/utilities.hpp"

LayoutBuilder::LayoutBuilder(const ParsedLayout& params) :
    m_params(params)
{
}

std::unique_ptr<MainLayout> LayoutBuilder::build()
{
    auto layout = create_from_params();

    for(auto region : m_params.regions)
    {
        layout->add_region(RegionBuilder(region).set_width_scale_factor(layout->width_scale_factor())
                                                .set_height_scale_factor(layout->height_scale_factor())
                                                .build());
    }
    return layout;
}

#include "XiboApp.hpp"
#include "control/Monitor.hpp"
#include <gdkmm/monitor.h>

std::unique_ptr<MainLayout> LayoutBuilder::create_from_params()
{
    std::string bgimage = m_params.bgimage.value_or(std::string{});
    std::string bgcolor = m_params.bgcolor.value_or(std::string{});

    auto layout = std::make_unique<MainLayout>(m_params.schemaVersion, m_params.width, m_params.height);

//    std::shared_ptr<IMonitor> monitor = std::make_shared<Monitor>(*layout);
//    layout->scale_to_monitor_size(std::move(monitor));

    // NOTE image has higher priority
    std::unique_ptr<IBackground> background = std::make_unique<Background>(layout->width(), layout->height());
    if(!bgcolor.empty())
        background->set_color(utilities::to_hex(bgcolor));

    if(!bgimage.empty())
        background->set_image(utilities::example_dir() + "/" + bgimage);

    layout->set_background(std::move(background));

    return layout;
}

