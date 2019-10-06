#include "PlayerError.hpp"

#include <istream>

PlayerError::PlayerError(std::string_view domain, std::string_view message) : domain_(domain), message_(message) {}

PlayerError::operator bool() const noexcept
{
    return !domain_.empty() || !message_.empty();
}

const std::string& PlayerError::domain() const
{
    return domain_;
}

const std::string& PlayerError::message() const
{
    return message_;
}

std::ostream& operator<<(std::ostream& out, const PlayerError& error)
{
    return out << "[" << error.domain() << "] " << error.message();
}
