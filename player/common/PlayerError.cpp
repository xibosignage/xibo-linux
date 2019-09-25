#include "PlayerError.hpp"
#include "Utils.hpp"

#include <iostream>

template <>
std::string Utils::toString(PlayerError::Type type)
{
    switch (type)
    {
        case PlayerError::Type::Success: return "Success";
        case PlayerError::Type::HTTP: return "HTTP";
        case PlayerError::Type::SOAP: return "SOAP";
        case PlayerError::Type::CMS: return "CMS";
    }

    return "unknown";
}

PlayerError::PlayerError(PlayerError::Type type, std::string_view message) : m_type(type), m_message(message) {}

PlayerError::operator bool() const noexcept
{
    return m_type != PlayerError::Type::Success;
}

PlayerError::Type PlayerError::type() const noexcept
{
    return m_type;
}

std::string PlayerError::name() const
{
    return Utils::toString(m_type);
}

std::string PlayerError::message() const
{
    return m_message;
}

std::ostream& operator<<(std::ostream& out, const PlayerError& error)
{
    return out << "[" << error.name() << "] " << error.message();
}
