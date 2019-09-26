#include "PlayerError.hpp"
#include "Utils.hpp"

#include <istream>

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

PlayerError::PlayerError(PlayerError::Type type, std::string_view message) : type_(type), message_(message) {}

PlayerError::operator bool() const noexcept
{
    return type_ != PlayerError::Type::Success;
}

PlayerError::Type PlayerError::type() const noexcept
{
    return type_;
}

std::string PlayerError::name() const
{
    return Utils::toString(type_);
}

std::string PlayerError::message() const
{
    return message_;
}

std::ostream& operator<<(std::ostream& out, const PlayerError& error)
{
    return out << "[" << error.name() << "] " << error.message();
}
