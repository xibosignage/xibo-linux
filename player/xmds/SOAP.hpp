#ifndef SOAP_HPP
#define SOAP_HPP

#include <string>

namespace SOAP
{
    template<typename Response>
    Response createResponse(const std::string& soapResponse);

    template<typename Response>
    class ResponseParser;

    template<typename Response>
    class RequestSerializer;

}

#endif // SOAP_HPP
