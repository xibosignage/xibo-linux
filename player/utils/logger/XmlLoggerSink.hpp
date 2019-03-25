#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/details/file_helper.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <sstream>
#include <mutex>

#include "XmlLogsRepo.hpp"

template<typename Mutex>
class XmlLoggerSink : public spdlog::sinks::base_sink<Mutex>
{
public:
    XmlLoggerSink(XmlLogsRepo& registry) : m_registry(registry)
    {
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        writeMessageToFile(formatMessage(msg));
    }

    void flush_() override
    {
        m_registry.append(m_buffer);
        m_buffer.clear();
    }

private:
    void writeMessageToFile(const std::string& message)
    {
        m_buffer.append(message);
    }

    std::string formatMessage(const spdlog::details::log_msg& msg)
    {
        std::string message;

        message += "<log ";
        message += "date=\"" + formatDateTime(msg.time) + "\"";
        message += "category=\"" + formatLogLevel(msg.level) + "\">";
        message += "<thread>" + std::to_string(msg.thread_id) + "</thread>";
        message += "<message>" + fmt::to_string(msg.raw) + "</message>";
        message += "</log>";

        return message;
    }

    std::string formatDateTime(std::chrono::system_clock::time_point tp)
    {
        using namespace std::chrono;

        auto secondSinceEpoch = duration_cast<seconds>(tp.time_since_epoch()).count();
        return boost::posix_time::to_simple_string(boost::posix_time::seconds(secondSinceEpoch));
    }

    std::string formatLogLevel(spdlog::level::level_enum level)
    {
        switch(level)
        {
            case spdlog::level::trace:
            case spdlog::level::debug:
            case spdlog::level::info:
//                return "audit"; FIXME
            case spdlog::level::warn:
            case spdlog::level::err:
            case spdlog::level::critical:
            case spdlog::level::off:
                return "error";
        }
        return {};
    }

private:
    std::string m_buffer;
    XmlLogsRepo& m_registry;

};

using LoggerXmlSinkMt = XmlLoggerSink<std::mutex>;
using LoggerXmlSinkSt = XmlLoggerSink<spdlog::details::null_mutex>;
