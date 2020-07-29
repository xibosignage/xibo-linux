#pragma once

#include <mutex>
#include <spdlog/details/file_helper.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "XmlLogsRepo.hpp"
#include "common/dt/DateTime.hpp"

template <typename Mutex>
class XmlLoggerSink : public spdlog::sinks::base_sink<Mutex>
{
public:
    XmlLoggerSink(XmlLogsRepo& registry) : registry_(registry) {}

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        writeMessageToFile(formatMessage(msg));
    }

    void flush_() override
    {
        registry_.append(buffer_);
        buffer_.clear();
    }

private:
    void writeMessageToFile(const std::string& message)
    {
        buffer_.append(message);
    }

    std::string formatMessage(const spdlog::details::log_msg& msg)
    {
        std::string message;

        message += "<log ";
        message += "date=\"" + formatDateTime(msg.time) + "\"";
        message += "category=\"" + formatLogLevel(msg.level) + "\">";
        message += "<thread>" + std::to_string(msg.thread_id) + "</thread>";
        message += "<message>" + fmt::to_string(msg.payload) + "</message>";
        message += "</log>";

        return message;
    }

    std::string formatDateTime(std::chrono::system_clock::time_point tp)
    {
        auto dt = DateTime::localFromTimestamp(std::chrono::system_clock::to_time_t(tp));

        return dt.string("%Y-%m-%d %H:%M:%S");
    }

    std::string formatLogLevel(spdlog::level::level_enum level)
    {
        switch (level)
        {
            case spdlog::level::trace: return "trace";
            case spdlog::level::debug: return "debug";
            case spdlog::level::info:
            case spdlog::level::warn: return "info";
            case spdlog::level::err:
            case spdlog::level::critical:
            case spdlog::level::off: return "error";
        }
        return {};
    }

private:
    std::string buffer_;
    XmlLogsRepo& registry_;
};

using LoggerXmlSinkMt = XmlLoggerSink<std::mutex>;
using LoggerXmlSinkSt = XmlLoggerSink<spdlog::details::null_mutex>;
