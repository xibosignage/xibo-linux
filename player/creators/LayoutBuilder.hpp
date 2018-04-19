#pragma once

#include <memory>
#include "constants.hpp"

class MainLayout;
class ParsedLayout;

class LayoutBuilder
{
public:
    LayoutBuilder(const ParsedLayout& params);
    std::unique_ptr<MainLayout> build();

private:
    std::unique_ptr<MainLayout> create_from_params();

private:
    const ParsedLayout& m_params;

};
