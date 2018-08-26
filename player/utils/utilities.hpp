#pragma once

#include <cstdint>
#include <string>
#include "constants.hpp"

class MediaParser;
class MainLayout;
class XMDSManager;

namespace utils
{
    uint32_t to_hex(const std::string& str_color);
    XMDSManager& xmds_manager();
    const std::string& example_dir();
    std::string app_current_dir();
    std::unique_ptr<MediaParser> get_media_parser(const xlf_node& parent_node, const xlf_node& media_node);
    std::unique_ptr<MainLayout> parse_xlf_layout(const std::string& xlf_path);

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
