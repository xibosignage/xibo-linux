#pragma once

#include <string>
#include <spdlog/logger.h>
#include <filesystem>

#include "constants.hpp"

class XMDSManager;
class DownloadManager;

namespace Utils
{
    std::shared_ptr<spdlog::logger> logger();
    XMDSManager& xmdsManager();
    DownloadManager& downloadManager();
    boost::property_tree::ptree parseXmlFromPath(const std::filesystem::path& xlfPath);
    boost::property_tree::ptree parseXmlFromString(const std::string& xml);
}
