#include "XmlLogsRepo.hpp"

std::string XmlLogsRepo::get() const
{
    return m_buffer;
}

void XmlLogsRepo::append(const std::string& data)
{
    std::unique_lock lock{m_mutex};
    m_buffer.append(data);
}

void XmlLogsRepo::clear()
{
    std::unique_lock lock{m_mutex};
    m_buffer.clear();
}
