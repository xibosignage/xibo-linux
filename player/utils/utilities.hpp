#pragma once

#include <cstdint>
#include <string>
#include <boost/filesystem/path.hpp>

#include "constants.hpp"

class MediaParser;
class MainLayout;
class XMDSManager;
class DownloadManager;

namespace utils
{
    XMDSManager& xmdsManager();
    DownloadManager& downloadManager();
    boost::filesystem::path resourcesDir();
    std::unique_ptr<MediaParser> getMediaParser(const xlf_node& parentNode, const xlf_node& mediaNode);
    std::unique_ptr<MainLayout> parseXlfLayout(const boost::filesystem::path& xlf_path);
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
