#pragma once

#include "constants.hpp"

class HttpManager;
class FileCacheManager;
class FilePath;
class ScreenShoter;

namespace Utils
{
    HttpManager& httpManager();
    FileCacheManager& fileManager();
    ScreenShoter& screenShoter();
    xml_node parseXmlFromPath(const FilePath& xlfPath);
    xml_node parseXmlFromString(const std::string& xml);
    std::string xmlTreeToEscapedString(const xml_node& node);
}
