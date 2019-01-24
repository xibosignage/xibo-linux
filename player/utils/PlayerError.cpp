#include "PlayerError.hpp"

#include <iostream>

PlayerError::PlayerError(PlayerError::Type type, std::string_view message) : m_type(type), m_message(message)
{
}

PlayerError::operator bool() const noexcept
{
    return m_type != PlayerError::Type::Success;
}

PlayerError::Type PlayerError::type() const noexcept
{
    return m_type;
}

std::string_view PlayerError::name() const
{
    switch(m_type)
    {
        case PlayerError::Type::HTTP:
            return "HTTP";
        case PlayerError::Type::SOAP:
            return "SOAP";
        default:
            break;
    }

    return {};
}

std::string_view PlayerError::message() const
{
    return m_message;
}

std::ostream& operator<<(std::ostream& out, const PlayerError& error)
{
    return out << "[" << error.name() << "] " << error.message();
}
