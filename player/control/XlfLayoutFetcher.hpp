#pragma once

#include "constants.hpp"

class IMainLayout;
class FilePath;
struct ParsedLayout;

class XlfLayoutFetcher
{
public:
    static std::unique_ptr<IMainLayout> fetch(int layoutId);

private:
    static FilePath getXlfPath(int layoutId);
    static ParsedLayout parseLayoutFromNode(const xml_node& node);
    static std::unique_ptr<IMainLayout> buildLayout(const ParsedLayout& parsedLayout);

};
