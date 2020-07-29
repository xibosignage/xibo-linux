#include "XmlLogsRepo.hpp"

XmlLogsRepo& XmlLogsRepo::get()
{
    static XmlLogsRepo repo;
    return repo;
}

std::string XmlLogsRepo::buffer() const
{
    return buffer_;
}

void XmlLogsRepo::append(const std::string& data)
{
    std::unique_lock lock{mutex_};
    buffer_.append(data);
}

void XmlLogsRepo::clear()
{
    std::unique_lock lock{mutex_};
    buffer_.clear();
}
