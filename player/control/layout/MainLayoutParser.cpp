#include "MainLayoutParser.hpp"

#include "control/layout/MainLayoutImpl.hpp"
#include "control/layout/MainLayoutResources.hpp"
#include "control/region/RegionParser.hpp"
#include "control/widgets/Image.hpp"

#include "common/fs/FilePath.hpp"
#include "common/fs/Resource.hpp"

const std::string DefaultColor = "#000";
const bool DefaultStatsEnabled = true;

MainLayoutParser::Error::Error(const std::string& domain, int layoutId, const std::string& reason) :
    PlayerRuntimeError{domain, "Layout " + std::to_string(layoutId) + " is invalid or missing. Reason: " + reason}
{
}

MainLayoutParser::MainLayoutParser(bool globalStatsEnabled) : globalStatsEnabled_{globalStatsEnabled} {}

std::unique_ptr<Xibo::MainLayout> MainLayoutParser::parseBy(int layoutId)
{
    try
    {
        layoutId_ = layoutId;

        Resource xlfFile{std::to_string(layoutId) + ".xlf"};
        auto root = Parsing::xmlFrom(xlfFile);
        return layoutFrom(root.get_child(XlfResources::LayoutNode));
    }
    catch (PlayerRuntimeError& e)
    {
        throw MainLayoutParser::Error{"LayoutParser - " + e.domain(), layoutId, e.message()};
    }
    catch (std::exception& e)
    {
        throw MainLayoutParser::Error{"LayoutParser", layoutId, e.what()};
    }
}

std::unique_ptr<Xibo::MainLayout> MainLayoutParser::layoutFrom(const XmlNode& node)
{
    auto options = optionsFrom(node);
    auto layout = std::make_unique<MainLayoutImpl>(options);

    layout->setBackground(createBackground(options));
    addRegions(*layout, node);

    return layout;
}

MainLayoutOptions MainLayoutParser::optionsFrom(const XmlNode& node)
{
    auto width = node.get<int>(XlfResources::MainLayout::Width);
    auto height = node.get<int>(XlfResources::MainLayout::Height);
    auto statsEnabled =
        globalStatsEnabled_ && node.get<bool>(XlfResources::MainLayout::StatsEnabled, DefaultStatsEnabled);

    auto backgroundUri = backgroundUriFrom(node);
    auto backgroundColor = backgroundColorFrom(node);

    return MainLayoutOptions{layoutId_, width, height, statsEnabled, backgroundUri, backgroundColor};
}

boost::optional<Uri> MainLayoutParser::backgroundUriFrom(const XmlNode& node)
{
    auto uri = node.get_optional<std::string>(XlfResources::MainLayout::BackgroundPath);
    if (uri)
    {
        return Uri::fromFile(Resource{uri.value()});
    }
    return {};
}

Color MainLayoutParser::backgroundColorFrom(const XmlNode& node)
{
    auto color = node.get<std::string>(XlfResources::MainLayout::BackgroundColor);

    return Color::fromString(color.empty() ? DefaultColor : color);
}

std::shared_ptr<Xibo::Image> MainLayoutParser::createBackground(const MainLayoutOptions& options)
{
    if (options.backgroundUri)
        return ImageWidgetFactory::create(
            options.backgroundUri.value(), options.width, options.height, Xibo::Image::PreserveRatio::False);
    else
        return ImageWidgetFactory::create(options.backgroundColor, options.width, options.height);
}

void MainLayoutParser::addRegions(Xibo::MainLayout& layout, const XmlNode& layoutNode)
{
    for (auto [nodeName, node] : layoutNode)
    {
        if (nodeName != XlfResources::RegionNode) continue;

        RegionParser parser{globalStatsEnabled_};
        auto position = parser.positionFrom(node);
        layout.addRegion(parser.regionFrom(node), position.left, position.top, position.zorder);
    }
}
