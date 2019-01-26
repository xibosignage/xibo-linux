#pragma once

#include "constants.hpp"

class XMDSManager;
class HTTPManager;
class FilePath;

namespace Utils
{
    XMDSManager& xmdsManager();
    HTTPManager& httpManager();
    xml_node parseXmlFromPath(const FilePath& xlfPath);
    xml_node parseXmlFromString(const std::string& xml);
    std::string xmlTreeToEscapedString(const xml_node& node);
}
