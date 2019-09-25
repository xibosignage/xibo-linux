#pragma once

#include "Logger.hpp"

namespace Log
{
std::shared_ptr<XiboLogger> logger();

template <typename... Args>
void trace(const char* fmt, const Args&... args)
{
    logger()->log(LoggingLevel::Trace, fmt, args...);
}

template <typename T>
void trace(const T& arg)
{
    logger()->log(LoggingLevel::Trace, arg);
}

template <typename... Args>
void debug(const char* fmt, const Args&... args)
{
    logger()->log(LoggingLevel::Debug, fmt, args...);
}

template <typename T>
void debug(const T& arg)
{
    logger()->log(LoggingLevel::Debug, arg);
}

template <typename... Args>
void info(const char* fmt, const Args&... args)
{
    logger()->log(LoggingLevel::Info, fmt, args...);
}

template <typename T>
void info(const T& arg)
{
    logger()->log(LoggingLevel::Info, arg);
}

template <typename... Args>
void error(const char* fmt, const Args&... args)
{
    logger()->log(LoggingLevel::Error, fmt, args...);
}

template <typename T>
void error(const T& arg)
{
    logger()->log(LoggingLevel::Error, arg);
}
}
