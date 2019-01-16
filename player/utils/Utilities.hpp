#pragma once

#include "constants.hpp"

class XMDSManager;
class HTTPDownloader;
class FilePath;

namespace Utils
{
    XMDSManager& xmdsManager();
    HTTPDownloader& httpDownloader();
    xlf_node parseXmlFromPath(const FilePath& xlfPath);
    xlf_node parseXmlFromString(const std::string& xml);
}
