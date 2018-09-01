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
    XMDSManager& xmds_manager();
    DownloadManager& download_manager();
    boost::filesystem::path resources_dir();
    std::unique_ptr<MediaParser> get_media_parser(const xlf_node& parent_node, const xlf_node& media_node);
    std::unique_ptr<MainLayout> parse_xlf_layout(const boost::filesystem::path& xlf_path);
    boost::property_tree::ptree parse_xml(const std::string& xml);

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
