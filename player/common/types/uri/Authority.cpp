#include "common/types/Uri.hpp"

Uri::Authority::Authority(const boost::optional<Uri::UserInfo>& userInfo,
                          const Uri::Host& host,
                          const boost::optional<Uri::Port>& port) :
    userInfo_(userInfo),
    host_(host),
    port_(port)
{
}

boost::optional<Uri::UserInfo> Uri::Authority::optionalUserInfo() const
{
    return userInfo_;
}

Uri::UserInfo Uri::Authority::userInfo() const
{
    assert(userInfo_);

    return userInfo_.value();
}

const Uri::Host& Uri::Authority::host() const
{
    return host_;
}

boost::optional<Uri::Port> Uri::Authority::optionalPort() const
{
    return port_;
}

Uri::Port Uri::Authority::port() const
{
    assert(port_);

    return port_.value();
}
