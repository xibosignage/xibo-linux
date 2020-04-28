#include "MediaInventory.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::MediaInventory;

std::string MediaInventory::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey, inventory);
}

void MediaInventory::Response::parseBody(const XmlNode& node)
{
    success = node.get<bool>(Resources::Success);
}
