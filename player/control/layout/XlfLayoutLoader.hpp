#pragma once

#include "constants.hpp"

#include "IMainLayout.hpp"
#include "MainLayoutParser.hpp"
#include "OverlayLayoutParser.hpp"

#include "../common/XlfResources.hpp"
#include "common/Parsing.hpp"
#include "common/fs/Resources.hpp"

template <typename ParserType>
class XlfLayoutLoader
{
public:
    static std::unique_ptr<IMainLayout> loadBy(int layoutId)
    {
        using namespace std::string_literals;

        try
        {
            auto rootNode = Parsing::xmlFromPath(getXlfPath(layoutId));
            return layoutFrom(rootNode);
        }
        catch (std::exception& e)
        {
            std::string error = "Layout (XLF file) is invalid or missing. Details: "s + e.what();
            throw std::runtime_error(error);
        }
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
