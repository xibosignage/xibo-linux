#pragma once

#include <memory>
#include "constants.hpp"

class MainLayout;
class ParsedLayout;

class LayoutBuilder
{
public:
    LayoutBuilder() = delete;
    static std::unique_ptr<MainLayout> create(const ParsedLayout& object);

private:
    static std::unique_ptr<MainLayout> create_from_params(const ParsedLayout& object);


};
