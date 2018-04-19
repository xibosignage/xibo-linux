#include "LayoutBuilder.hpp"
#include "RegionBuilder.hpp"

#include "parsers/LayoutParser.hpp"
#include "parsers/ParserHelpers.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"

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

std::unique_ptr<MainLayout> LayoutBuilder::create_from_params()
{
    std::string bgimage = m_params.bgimage.value_or(std::string{});
    std::string bgcolor = m_params.bgcolor.value_or(std::string{});

    return std::make_unique<MainLayout>(m_params.schemaVersion, m_params.width, m_params.height, bgimage, bgcolor);
}

