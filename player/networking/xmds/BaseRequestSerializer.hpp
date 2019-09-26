#pragma once

#include <sstream>
#include <string_view>

namespace Soap
{
    template <typename Request>
    class BaseRequestSerializer
    {
    public:
        BaseRequestSerializer(const Request& request) : m_request(request) {}

    protected:
        template <typename... Args>
        std::string createRequest(std::string_view requestName, Args... fields)
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

        const Request& request() const
        {
            return m_request;
        }

    private:
        const Request& m_request;
    };
}
