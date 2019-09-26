#pragma once

#include <string>

class PlayerError
{
public:
    enum class Type
    {
        Success = 0,
        HTTP,
        SOAP,
        CMS
    };

    PlayerError() = default;
    PlayerError(Type type, std::string_view message);

    explicit operator bool() const noexcept;
    Type type() const noexcept;
    std::string name() const;
    std::string message() const;

    friend std::ostream& operator<<(std::ostream& out, const PlayerError& error);

private:
    Type m_type = PlayerError::Type::Success;
    std::string m_message;
};
