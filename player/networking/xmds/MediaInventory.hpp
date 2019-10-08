#pragma once

#include "networking/xmds/BaseRequestSerializer.hpp"
#include "networking/xmds/BaseResponseParser.hpp"
#include "networking/xmds/MediaInventoryField.hpp"
#include "networking/xmds/Soap.hpp"

#include "common/Field.hpp"

namespace MediaInventory
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<MediaInventoryItems> inventory{"mediaInventory"};
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
