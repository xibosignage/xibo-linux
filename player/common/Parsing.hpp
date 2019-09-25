#pragma once

#include "constants.hpp"

class FilePath;

namespace Parsing
{
ptree_node xmlFromPath(const FilePath& xlfPath);
ptree_node xmlFromString(const std::string& xml);
ptree_node jsonFromString(const std::string& json);
std::string xmlTreeToString(const ptree_node& node);
std::string xmlTreeToEscapedString(const ptree_node& node);
std::string xmlAttr(const std::string& property);
}
