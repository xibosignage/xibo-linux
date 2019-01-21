#pragma once

#include "constants.hpp"

class XMDSManager;
class HTTPManager;
class FilePath;

namespace Utils
{
    XMDSManager& xmdsManager();
    HTTPManager& httpManager();
    xlf_node parseXmlFromPath(const FilePath& xlfPath);
    xlf_node parseXmlFromString(const std::string& xml);
}
