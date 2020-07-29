#pragma once

#include <string>

class PlayerError
{
public:
    PlayerError() = default;
    PlayerError(std::string_view domain, std::string_view message);

    explicit operator bool() const noexcept;
    const std::string& domain() const;
    const std::string& message() const;

    friend std::ostream& operator<<(std::ostream& out, const PlayerError& error);

private:
    std::string domain_;
    std::string message_;
};
