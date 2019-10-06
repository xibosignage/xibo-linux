#pragma once

#include <boost/optional/optional.hpp>
#include <regex>
#include <string>

#include "common/PlayerRuntimeError.hpp"
#include "common/types/Uri.hpp"

class UriParser
{
public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    Uri parse(const std::string& rawUri);

private:
    Uri::Scheme schemeFrom(const std::smatch& results);
    boost::optional<Uri::Authority> authorityFrom(const std::smatch& results);
    boost::optional<Uri::UserInfo> userInfoFrom(const std::smatch& results);
    boost::optional<Uri::Host> hostFrom(const std::smatch& results);
    boost::optional<Uri::Port> portFrom(const std::smatch& results);
    std::string pathFrom(const std::smatch& results);

    std::smatch parseInternal(const std::string& uri);
};
