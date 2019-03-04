#pragma once

#include <string>
#include <mutex>

class XmlLogsRepo
{
public:
    static XmlLogsRepo& get();

    std::string buffer() const;
    void append(const std::string& data);
    void clear();

private:
    std::string m_buffer;
    std::mutex m_mutex;

};
