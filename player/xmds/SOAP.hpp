#ifndef SOAP_HPP
#define SOAP_HPP

#include <string_view>
#include <sstream>
#include <boost/property_tree/ptree.hpp>

namespace SOAP
{
    template<typename response>
    response createResponse(const std::string& soapResponse);

    class BaseResponseParser
    {
    public:
        BaseResponseParser(const std::string& xmlResponse);

    protected:
        const boost::property_tree::ptree& responseTree() const;

    private:
        boost::property_tree::ptree parseSoapResponse(const std::string& xmlResponse);

    private:
        boost::property_tree::ptree m_responseTree;

    };

    template<typename Request>
    class BaseRequestSerializer
    {
    public:
        BaseRequestSerializer(const Request& request) : m_request(request)
        {
        }

    protected:
        template<typename... Args>
        std::string createRequest(std::string_view requestName, Args... fields)
        {
            std::stringstream stream;
            stream << R"(<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:tns="urn:xmds" xmlns:types="urn:xmds/encodedTypes" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">)";
            stream << R"(<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">)";
            stream << "<tns:" << requestName <<  ">";
            ((stream << "<" << fields.name() << " xsi:type=\"xsd:" << fields.type() << "\">" << fields.value() << "</" << fields.name() << ">"), ...);
            stream << "</tns:" << requestName <<  ">";
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

    template<typename Response>
    class ResponseParser;

    template<typename Response>
    class RequestSerializer;

}

#endif // SOAP_HPP
