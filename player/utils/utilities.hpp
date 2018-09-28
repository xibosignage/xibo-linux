#pragma once

#include <string>
#include <spdlog/logger.h>
#include <filesystem>

#include "constants.hpp"

class XMDSManager;
class DownloadManager;

namespace utils
{
    std::shared_ptr<spdlog::logger> logger();
    XMDSManager& xmdsManager();
    DownloadManager& downloadManager();
    std::filesystem::path resourcesDir();
    boost::property_tree::ptree parseXmlFromPath(const std::filesystem::path& xlfPath);
    boost::property_tree::ptree parseXmlFromString(const std::string& xml);
    std::vector<uint8_t> getRawData(const std::filesystem::path& path);

    template <typename T>
    class Finalizer
    {
    public:
        Finalizer(T&& functor) :
            m_functor(std::move(functor))
        {
        }
        ~Finalizer()
        {
            m_functor();
        }

    private:
        T m_functor;
    };

    template<typename T>
    auto finally(T&& functor)
    {
        return Finalizer<T>(std::forward<T>(functor));
    }
}
