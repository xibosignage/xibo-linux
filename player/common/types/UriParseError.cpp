#include "UriParseError.hpp"

UriParseError::UriParseError(const std::string& message) : m_message(message) {}

const char* UriParseError::what() const noexcept
{
    return m_message.c_str();
}
