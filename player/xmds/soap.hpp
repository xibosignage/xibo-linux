#ifndef SOAP_HPP
#define SOAP_HPP

#include <boost/property_tree/ptree.hpp>

namespace soap
{
    boost::property_tree::ptree parse_soap_response(const std::string& xml_response);

    template<typename request>
    struct request_traits;

    template<typename response>
    response create_response(const std::string& soap_response);

    template<typename request, typename... Args>
    std::string create_request(Args... fields)
    {
        std::stringstream stream;
        stream << R"(<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:soapenc="http://schemas.xmlsoap.org/soap/encoding/" xmlns:tns="urn:xmds" xmlns:types="urn:xmds/encodedTypes" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">)";
        stream << R"(<soap:Body soap:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">)";
        stream << "<tns:" << request_traits<request>::name <<  ">";
        ((stream << "<" << fields.name() << " xsi:type=\"xsd:" << fields.type() << "\">" << fields.value() << "</" << fields.name() << ">"), ...);
        stream << "</tns:" << request_traits<request>::name <<  ">";
        stream << "</soap:Body>";
        stream << "</soap:Envelope>";

        return stream.str();
    }

    template<typename request>
    std::string request_string(const request& soap_request);
}

#endif // SOAP_HPP
