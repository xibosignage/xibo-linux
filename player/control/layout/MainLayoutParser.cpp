#include "MainLayoutParser.hpp"
#include "MainLayoutResources.hpp"
#include "MainLayout.hpp"

#include "control/region/RegionParser.hpp"
#include "control/common/Validators.hpp"
#include "control/common/Image.hpp"
#include "control/common/OverlayLayout.hpp"

#include "common/FilePath.hpp"
#include "utils/ColorToHexConverter.hpp"

const std::string DefaultColor = "#000";

std::unique_ptr<IMainLayout> MainLayoutParser::layoutFrom(const xml_node& node)
{
    auto options = optionsFrom(node);
    auto view = createView(options);
    auto layout = createLayout(view);

    addRegions(*layout, node);

    return layout;
}

std::unique_ptr<IMainLayout> MainLayoutParser::createLayout(const std::shared_ptr<IOverlayLayout>& view)
{
    return std::make_unique<MainLayout>(view);
}

LayoutOptions MainLayoutParser::optionsFrom(const xml_node& node)
{
    LayoutOptions options;

    options.width = node.get<int>(ResourcesXlf::MainLayout::Width);
    options.height = node.get<int>(ResourcesXlf::MainLayout::Height);

    options.backgroundUri = uriFrom(node);
    options.backgroundColor = colorFrom(node);

    return options;
}

Uri MainLayoutParser::uriFrom(const xml_node& node)
{
    auto uri = node.get_optional<std::string>(ResourcesXlf::MainLayout::BackgroundPath);

    return Validators::validateUri(uri);
}

uint32_t MainLayoutParser::colorFrom(const xml_node& node)
{
    auto color = node.get<std::string>(ResourcesXlf::MainLayout::BackgroundColor);

    color = color.empty() ? DefaultColor : color;

    ColorToHexConverter converter;
    return converter.colorToHex(color);
}

std::shared_ptr<IOverlayLayout> MainLayoutParser::createView(const LayoutOptions& options)
{
    auto view = std::make_shared<OverlayLayout>(options.width, options.height);

    view->setMainChild(createBackground(options));

    return view;
}

std::shared_ptr<IImage> MainLayoutParser::createBackground(const LayoutOptions& options)
{
    auto background = std::make_shared<Image>(options.width, options.height);

    if(options.backgroundUri.isValid())
        background->loadFromFile(options.backgroundUri.path(), false);
    else
        background->setColor(options.backgroundColor);

    return background;
}

void MainLayoutParser::addRegions(IMainLayout& layout, const xml_node& layoutNode)
{
    for(auto [nodeName, node] : layoutNode)
    {
        if(nodeName != ResourcesXlf::RegionNode) continue;

        RegionParser parser;
        auto options = parser.optionsFrom(node);
        layout.addRegion(parser.regionFrom(node), options.left, options.top, options.zindex);
    }
}
