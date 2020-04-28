#pragma once

#include <exception>
#include <string>

#include "common/PlayerError.hpp"

class PlayerRuntimeError : public std::exception
{
public:
    PlayerRuntimeError(const std::string& domain, const std::string& message) :
        domain_(domain),
        message_(message),
        what_("[" + domain_ + "] " + message_)
    {
    }

    PlayerRuntimeError(const PlayerError& player) : PlayerRuntimeError{player.domain(), player.message()} {}

    const char* what() const noexcept override
    {
        return what_.data();
    }

    PlayerError error() const
    {
        return PlayerError{domain_, message_};
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
