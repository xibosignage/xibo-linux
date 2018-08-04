#include "xmds.hpp"
#include "utils.hpp"
#include "soap.hpp"

boost::property_tree::ptree xmds::parse_xml_response(const std::string& soap_response)
{
    auto response_tree = soap::parse_soap_response(soap_response);
    auto [_, message_node] = response_tree.front();
    return utils::parse_xml(message_node.get_value<std::string>());
}

bool xmds::parse_success_response(const std::string& soap_response)
{
    auto response_tree = soap::parse_soap_response(soap_response);
    return response_tree.get_child("success").get_value<bool>();
}

std::string xmds::parse_file_response(const std::string& soap_response)
{
    auto response_tree = soap::parse_soap_response(soap_response);
    return response_tree.get_child("file").get_value<std::string>();
}

