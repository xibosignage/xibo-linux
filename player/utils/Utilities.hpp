#pragma once

#include <spdlog/logger.h>
#include "constants.hpp"

class XMDSManager;
class HTTPDownloader;
class FilePath;

namespace Utils
{
    std::shared_ptr<spdlog::logger> logger();
    XMDSManager& xmdsManager();
    HTTPDownloader& httpDownloader();
    xlf_node parseXmlFromPath(const FilePath& xlfPath);
    xlf_node parseXmlFromString(const std::string& xml);
}
