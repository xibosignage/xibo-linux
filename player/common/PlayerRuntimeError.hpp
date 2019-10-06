#pragma once

#include <exception>
#include <string>

class PlayerRuntimeError : public std::exception
{
public:
    PlayerRuntimeError(std::string_view domain, std::string_view message) :
        domain_(domain),
        message_(message),
        what_("[" + domain_ + "] " + message_)
    {
    }

    const char* what() const noexcept override
    {
        return what_.data();
    }

    const std::string& domain() const
    {
        return domain_;
    }

    const std::string& message() const
    {
        return message_;
    }

private:
    std::string domain_;
    std::string message_;
    std::string what_;
};
