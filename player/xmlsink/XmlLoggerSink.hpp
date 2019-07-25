#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/details/file_helper.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
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
        std::stringstream stream;

        auto facet = new boost::posix_time::time_facet();
        facet->format("%Y-%m-%d %H:%M:%S");

        stream.imbue(std::locale(std::locale::classic(), facet));
        stream << local_ptime_from_utc_time_t(std::chrono::system_clock::to_time_t(tp));

        return stream.str();
    }

    boost::posix_time::ptime local_ptime_from_utc_time_t(std::time_t const t)
    {
        using boost::date_time::c_local_adjustor;
        using boost::posix_time::from_time_t;
        using boost::posix_time::ptime;
        return c_local_adjustor<ptime>::utc_to_local(from_time_t(t));
    }

    std::string formatLogLevel(spdlog::level::level_enum level)
    {
        switch(level)
        {
            case spdlog::level::trace:
                return "trace";
            case spdlog::level::debug:
                return "debug";
            case spdlog::level::info:
            case spdlog::level::warn:
                return "info";
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
