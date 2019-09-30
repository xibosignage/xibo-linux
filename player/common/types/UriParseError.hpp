#pragma once

#include <exception>
#include <string>

class UriParseError : public std::exception
{
public:
    UriParseError(const std::string& message);
    const char* what() const noexcept;

private:
    std::string m_message;
};
