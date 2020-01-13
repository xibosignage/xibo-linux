#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/MediaInventoryField.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace MediaInventory
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<MediaInventoryItems> inventory{"mediaInventory"};
    };
}

template <>
class Soap::RequestSerializer<MediaInventory::Request> : public BaseRequestSerializer<MediaInventory::Request>
{
public:
    RequestSerializer(const MediaInventory::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<MediaInventory::Result> : public BaseResponseParser<MediaInventory::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    MediaInventory::Result parseBody(const XmlNode& node) override;
};
