#include "soap.hpp"
#include "utils/Utilities.hpp"
#include <iostream>

// FIXME
// return error code
// or 2 versions (exceptions and error code passing)
boost::property_tree::ptree soap::parseSoapResponse(const std::string& xmlResponse)
{
    auto parsedBody = Utils::parseXmlFromString(xmlResponse).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
    if(auto faultNode = parsedBody.get_child_optional("SOAP-ENV:Fault"))
    {
        auto faultCode = faultNode->get<std::string>("faultcode");
        auto faultMessage = faultNode->get<std::string>("faultstring");
        std::cout << "Fault Response" << std::endl;
        std::cout << faultCode << " " << faultMessage << std::endl;
        return {};
    }
    return parsedBody.front().second;
}
