#pragma once

#include "constants.hpp"

#include "MainLayoutParser.hpp"
#include "OverlayLayoutParser.hpp"
#include "IMainLayout.hpp"

#include "../common/XlfResources.hpp"
#include "common/Parsing.hpp"
#include "utils/Resources.hpp"

template<typename ParserType>
class XlfLayoutLoader
{
public:
    static std::unique_ptr<IMainLayout> loadBy(int layoutId)
    {
        auto rootNode = Parsing::xmlFromPath(getXlfPath(layoutId));
        return layoutFrom(rootNode);
    }

private:
    static FilePath getXlfPath(int layoutId)
    {
        auto xlfFile = std::to_string(layoutId) + ".xlf";
        return Resources::directory() / xlfFile;
    }

    static std::unique_ptr<IMainLayout> layoutFrom(const ptree_node& root)
    {
        auto node = root.get_child(XlfResources::LayoutNode);

        ParserType parser;
        return parser.layoutFrom(node);
    }
};

using XlfMainLayoutLoader = XlfLayoutLoader<MainLayoutParser>;
using XlfOverlayLayoutLoader = XlfLayoutLoader<OverlayLayoutParser>;
