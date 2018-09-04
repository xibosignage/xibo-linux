#pragma once

#include <cstdint>
#include <string>
#include <spdlog/logger.h>
#include <filesystem>

#include "constants.hpp"

class MediaFactory;
class IMainLayout;
class XMDSManager;
class DownloadManager;

namespace utils
{
    std::shared_ptr<spdlog::logger> logger();
    XMDSManager& xmdsManager();
    DownloadManager& downloadManager();
    std::filesystem::path resourcesDir();
    std::unique_ptr<MediaFactory> getMediaFactory(const xlf_node& parentNode, const xlf_node& mediaNode);
    std::unique_ptr<IMainLayout> parseAndCreateXlfLayout(const std::filesystem::path& xlf_path);
    boost::property_tree::ptree parseXml(const std::string& xml);

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
