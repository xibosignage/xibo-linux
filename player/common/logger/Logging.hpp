#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/logger.h>

namespace Log
{
    std::shared_ptr<spdlog::logger> create();
    std::shared_ptr<spdlog::logger> create(const std::vector<spdlog::sink_ptr>& sinks);
    void setLevel(const std::string& level);
    std::shared_ptr<spdlog::logger> logger();

    template <typename... Args>
    void trace(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::trace, fmt, args...);
    }

    template <typename T>
    void trace(const T& arg)
    {
        logger()->log(spdlog::level::trace, arg);
    }

    template <typename... Args>
    void debug(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::debug, fmt, args...);
    }

    template <typename T>
    void debug(const T& arg)
    {
        logger()->log(spdlog::level::debug, arg);
    }

    template <typename... Args>
    void info(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::info, fmt, args...);
    }

    template <typename T>
    void info(const T& arg)
    {
        logger()->log(spdlog::level::info, arg);
    }

    template <typename... Args>
    void error(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::err, fmt, args...);
    }

    template <typename T>
    void error(const T& arg)
    {
        logger()->log(spdlog::level::err, arg);
    }
}
