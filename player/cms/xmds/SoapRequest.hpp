#pragma once

#include <sstream>
#include <string_view>

#include "cms/xmds/Soap.hpp"

template <typename Request>
class SoapRequest
{
public:
    template <typename Response>
    FutureResponseResult<Response> send(const Uri& uri)
    {
        return Soap::sendRequest<Response>(uri, static_cast<Request&>(*this));
    }

    virtual std::string string() const = 0;

protected:
    ~SoapRequest() = default;

    template <typename... Args>
    std::string toSoapString(std::string_view requestName, Args... fields) const
    {
        std::stringstream stream;
        stream
            << R"(<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:tns="urn:xmds" xmlns:types="urn:xmds/encodedTypes" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">)";
        stream << R"(<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">)";
        stream << "<tns:" << requestName << ">";
        ((stream << "<" << fields.name() << " xsi:type=\"xsd:" << fields.type() << "\">" << fields.value() << "</"
                 << fields.name() << ">"),
         ...);
        stream << "</tns:" << requestName << ">";
        stream << "</soap:Body>";
        stream << "</soap:Envelope>";

        return stream.str();
    }
};
