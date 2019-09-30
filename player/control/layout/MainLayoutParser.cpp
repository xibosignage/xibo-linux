#include "MainLayoutParser.hpp"

#include "control/layout/MainLayoutImpl.hpp"
#include "control/layout/MainLayoutResources.hpp"
#include "control/region/RegionParser.hpp"
#include "control/widgets/ImageWidgetFactory.hpp"

#include "common/fs/FilePath.hpp"
#include "common/types/ResourceFile.hpp"

const std::string DefaultColor = "#000";

using namespace std::string_literals;

std::unique_ptr<Xibo::MainLayout> MainLayoutParser::parseBy(int layoutId)
{
    try
    {
        ResourceFile xlfFile{std::to_string(layoutId) + ".xlf"};
        auto root = Parsing::xmlFromPath(xlfFile);
        return layoutFrom(root.get_child(XlfResources::LayoutNode));
    }
    catch (std::exception& e)
    {
        std::string message = "Layout " + std::to_string(layoutId) + " is invalid or missing. Reason: ";
        throw MainLayoutParser::Error(message + e.what());
    }
}

std::unique_ptr<Xibo::MainLayout> MainLayoutParser::layoutFrom(const PtreeNode& node)
{
    auto options = optionsFrom(node);
    auto layout = std::make_unique<MainLayoutImpl>(options);

    layout->setBackground(createBackground(options));
    addRegions(*layout, node);

    return layout;
}

MainLayoutOptions MainLayoutParser::optionsFrom(const PtreeNode& node)
{
    MainLayoutOptions options;

    options.width = node.get<int>(XlfResources::MainLayout::Width);
    options.height = node.get<int>(XlfResources::MainLayout::Height);

    options.backgroundUri = backgroundUriFrom(node);
    options.backgroundColor = backgroundColorFrom(node);

    return options;
}

Uri MainLayoutParser::backgroundUriFrom(const PtreeNode& node)
{
    auto uri = node.get_optional<std::string>(XlfResources::MainLayout::BackgroundPath);

    if (uri)
    {
        return Uri{ResourceFile{uri.value()}};
    }

    return {};
}

Color MainLayoutParser::backgroundColorFrom(const PtreeNode& node)
{
    auto color = node.get<std::string>(XlfResources::MainLayout::BackgroundColor);

    return Color::fromString(color.empty() ? DefaultColor : color);
}

std::shared_ptr<Xibo::Image> MainLayoutParser::createBackground(const MainLayoutOptions& options)
{
    auto background = ImageWidgetFactory::create(options.width, options.height);

    // TODO: remove isValid
    if (options.backgroundUri.isValid())
        background->loadFrom(options.backgroundUri, Xibo::Image::PreserveRatio::False);
    else
        background->setColor(options.backgroundColor);

    return background;
}

void MainLayoutParser::addRegions(Xibo::MainLayout& layout, const PtreeNode& layoutNode)
{
    for (auto [nodeName, node] : layoutNode)
    {
        if (nodeName != XlfResources::RegionNode) continue;

        RegionParser parser;
        auto position = parser.positionFrom(node);
        layout.addRegion(parser.regionFrom(node), position.left, position.top, position.zorder);
    }
}
