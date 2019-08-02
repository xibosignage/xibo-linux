#pragma once

#include "constants.hpp"

#include "MainLayoutParser.hpp"
#include "OverlayLayoutParser.hpp"
#include "IMainLayout.hpp"

#include "../common/XlfResources.hpp"
#include "common/Utils.hpp"
#include "utils/Resources.hpp"

template<typename ParserType>
class XlfLayoutFetcher
{
public:
    static std::unique_ptr<IMainLayout> fetch(int layoutId)
    {
        auto rootNode = Utils::parseXmlFromPath(getXlfPath(layoutId));
        return layoutFrom(rootNode);
    }

private:
    static FilePath getXlfPath(int layoutId)
    {
        auto xlfFile = std::to_string(layoutId) + ".xlf";
        return Resources::directory() / xlfFile;
    }

    static std::unique_ptr<IMainLayout> layoutFrom(const xml_node& root)
    {
        auto node = root.get_child(ResourcesXlf::LayoutNode);

        ParserType parser;
        return parser.layoutFrom(node);
    }
};

using XlfMainLayoutFetcher = XlfLayoutFetcher<MainLayoutParser>;
using XlfOverlayLayoutFetcher = XlfLayoutFetcher<OverlayLayoutParser>;
