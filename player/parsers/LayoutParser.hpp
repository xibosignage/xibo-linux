#pragma once

#include "constants.hpp"
#include "parsers/ParserHelpers.hpp"
#include <spdlog/spdlog.h>

class LayoutParser
{
public:
    LayoutParser(const std::string& path);
    ParsedLayout parse_layout();

private:
    ParsedLayout parse_layout_params();

private:
    xlf_node m_layout_node;

};
