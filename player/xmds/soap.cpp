#include "soap.hpp"
#include "utils/utilities.hpp"
#include <iostream>

// FIXME
// return error code
// or 2 versions (exceptions and error code passing)
boost::property_tree::ptree soap::parse_soap_response(const std::string& xml_response)
{
    auto parsed_body = utils::parse_xml(xml_response).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
    if(auto fault_node = parsed_body.get_child_optional("SOAP-ENV:Fault"))
    {
        auto fault_code = fault_node->get<std::string>("faultcode");
        auto fault_message = fault_node->get<std::string>("faultstring");
        std::cout << "Fault Response" << std::endl;
        std::cout << fault_code << " " << fault_message << std::endl;
        return {};
    }
    return parsed_body.front().second;
}
