#include "XlfLayoutFetcher.hpp"

#include "layout/MainLayoutBuilder.hpp"
#include "layout/MainLayoutParser.hpp"
#include "layout/IMainLayout.hpp"

#include "common/XlfResources.hpp"
#include "common/Utils.hpp"
#include "utils/Resources.hpp"

std::unique_ptr<IMainLayout> XlfLayoutFetcher::fetch(int layoutId)
{
    auto rootNode = Utils::parseXmlFromPath(getXlfPath(layoutId));
    auto parsedLayout = parseLayoutFromNode(rootNode);

    return buildLayout(parsedLayout);
}

FilePath XlfLayoutFetcher::getXlfPath(int layoutId)
{
    auto xlfFile = std::to_string(layoutId) + ".xlf";
    return Resources::directory() / xlfFile;
}

ParsedLayout XlfLayoutFetcher::parseLayoutFromNode(const xml_node& node)
{
    auto layoutNode = node.get_child(ResourcesXlf::LayoutNode);

    MainLayoutParser parser;
    return parser.parse(layoutNode);
}

std::unique_ptr<IMainLayout> XlfLayoutFetcher::buildLayout(const ParsedLayout& parsedLayout)
{
    MainLayoutBuilder builder;

    return builder.build(parsedLayout);
}
