#pragma once

#include <spdlog/logger.h>
#include "constants.hpp"

class XMDSManager;
class DownloadManager;
class FilePath;

namespace Utils
{
    std::shared_ptr<spdlog::logger> logger();
    XMDSManager& xmdsManager();
    DownloadManager& downloadManager();
    xlf_node parseXmlFromPath(const FilePath& xlfPath);
    xlf_node parseXmlFromString(const std::string& xml);
}
