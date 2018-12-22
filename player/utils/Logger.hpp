#pragma once

#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Log
{
    inline std::shared_ptr<spdlog::logger> logger()
    {
        static auto logger = spdlog::get(LOGGER);
        return logger;
    }

    template<typename... Args>
    void trace(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::trace, fmt, args...);
    }

    template<typename T>
    void trace(const T& arg)
    {
        logger()->log(spdlog::level::trace, arg);
    }

    template<typename... Args>
    void debug(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::debug, fmt, args...);
    }

    template<typename T>
    void debug(const T& arg)
    {
        logger()->log(spdlog::level::debug, arg);
    }

    template<typename... Args>
    void info(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::info, fmt, args...);
    }

    template<typename T>
    void info(const T& arg)
    {
        logger()->log(spdlog::level::info, arg);
    }

    template<typename... Args>
    void warn(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::warn, fmt, args...);
    }

    template<typename T>
    void warn(const T& arg)
    {
        logger()->log(spdlog::level::warn, arg);
    }

    template<typename... Args>
    void error(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::err, fmt, args...);
    }

    template<typename T>
    void error(const T& arg)
    {
        logger()->log(spdlog::level::err, arg);
    }

    template<typename... Args>
    void critical(const char* fmt, const Args&... args)
    {
        logger()->log(spdlog::level::critical, fmt, args...);
    }

    template<typename T>
    void critical(const T& arg)
    {
        logger()->log(spdlog::level::critical, arg);
    }
}
