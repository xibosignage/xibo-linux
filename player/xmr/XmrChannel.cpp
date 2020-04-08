#include "XmrChannel.hpp"

#include "common/crypto/Md5Hash.hpp"

XmrChannel XmrChannel::fromCmsSettings(const std::string& host,
                                       const std::string& cmsKey,
                                       const std::string& hardwareKey)
{
    auto hash = Md5Hash::fromString(host + cmsKey + hardwareKey);
    return XmrChannel{static_cast<std::string>(hash)};
}
