#pragma once

#include <string>
#include <mutex>

class XmlLogsRepo
{
public:
    std::string get() const;
    void append(const std::string& data);
    void clear();

private:
    std::string m_buffer;
    std::mutex m_mutex;

};
