#pragma once

#include <cstdint>
#include <string>

namespace utilities
{
    uint32_t to_hex(const std::string& str_color);
    std::string example_dir();
    std::string xlf_file();
    std::string app_current_dir();

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
