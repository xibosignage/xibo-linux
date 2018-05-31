#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>

class MainLayout;

class LayoutParser
{
public:
    LayoutParser(const xlf_node& region_node);
    std::unique_ptr<MainLayout> parse();
    static boost::property_tree::ptree get_layout_node(const std::string& xlf_path);

private:
    struct ParsedLayout
    {
        int schemaVersion;
        int width;
        int height;
        std::string bgimage;
        std::string bgcolor;
    };

    ParsedLayout parse_params();

private:
    xlf_node m_layout_node;

};
