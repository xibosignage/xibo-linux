#include "common/types/Uri.hpp"

#include <boost/algorithm/string.hpp>

Uri::UserInfo::UserInfo(const std::string& username, const std::string& password) :
    username_(username),
    password_(password)
{
    if (username.empty()) throw Error{"Uri::UserInfo", "Username can't be empty"};

    value_ = password.empty() ? username : username + ":" + password;
}

Uri::UserInfo Uri::UserInfo::fromString(const std::string& userinfo)
{
    const size_t UserName = 0;
    const size_t Password = 1;
    std::vector<std::string> splittedUserinfo;
    boost::split(splittedUserinfo, userinfo, boost::is_any_of(":"));

    if (splittedUserinfo.size() == 1) return Uri::UserInfo{splittedUserinfo[UserName], std::string{}};
    if (splittedUserinfo.size() == 2) return Uri::UserInfo{splittedUserinfo[UserName], splittedUserinfo[Password]};

    throw Error{"Uri::UserInfo", "Parse error"};
}

const std::string& Uri::UserInfo::username() const
{
    return username_;
}

const std::string& Uri::UserInfo::password() const
{
    return password_;
}

bool operator==(const Uri::UserInfo& first, const Uri::UserInfo& second)
{
    return static_cast<std::string>(first) == static_cast<std::string>(second);
}

bool operator!=(const Uri::UserInfo& first, const Uri::UserInfo& second)
{
    return !(first == second);
}
