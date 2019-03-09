#pragma once

#include "utils/Field.hpp"
#include "MediaInventoryField.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

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

template<>
class SOAP::RequestSerializer<MediaInventory::Request> : public BaseRequestSerializer<MediaInventory::Request>
{
public:
    RequestSerializer(const MediaInventory::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<MediaInventory::Result> : public BaseResponseParser<MediaInventory::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    MediaInventory::Result doParse(const xml_node& node) override;

};
