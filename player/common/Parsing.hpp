#pragma once

#include "constants.hpp"

class FilePath;

namespace Parsing
{
    PtreeNode xmlFromPath(const FilePath& xlfPath);
    PtreeNode xmlFromString(const std::string& xml);
    PtreeNode jsonFromString(const std::string& json);
    std::string xmlTreeToString(const PtreeNode& node);
    std::string xmlTreeToEscapedString(const PtreeNode& node);
    std::string xmlAttr(const std::string& property);
    std::string xmlOption(const std::string& property);
}
