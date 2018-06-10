#pragma once

#include "constants.hpp"
#include <spdlog/spdlog.h>
#include <gtest/gtest_prod.h>

class MainLayout;

class LayoutParser
{
public:
    LayoutParser(const xlf_node& layout_node);
    std::unique_ptr<MainLayout> parse();
    std::vector<xlf_node>::const_iterator begin() const;
    std::vector<xlf_node>::const_iterator end() const;

private:
    xlf_node m_layout_node;
    std::vector<xlf_node> m_region_nodes;

};
