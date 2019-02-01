#pragma once

#include "constants.hpp"

class XMDSManager;
class HTTPManager;
class FileCacheManager;
class FilePath;

namespace Utils
{
    XMDSManager& xmdsManager();
    HTTPManager& httpManager();
    FileCacheManager& fileManager();
    xml_node parseXmlFromPath(const FilePath& xlfPath);
    xml_node parseXmlFromString(const std::string& xml);
    std::string xmlTreeToEscapedString(const xml_node& node);
}
