#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/MediaInventoryField.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct MediaInventory
{
    struct Response
    {
        bool success;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<MediaInventoryItems> inventory{"mediaInventory"};
    };
};

template <>
class Soap::RequestSerializer<MediaInventory::Request> : public BaseRequestSerializer<MediaInventory::Request>
{
public:
    RequestSerializer(const MediaInventory::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<MediaInventory::Response> : public BaseResponseParser<MediaInventory::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    MediaInventory::Response parseBody(const XmlNode& node) override;
};
