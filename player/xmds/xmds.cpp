#include "xmds.hpp"
#include "soap.hpp"
#include "utils/Utilities.hpp"

boost::property_tree::ptree xmds::parseXmlResponse(const std::string& soapResponse)
{
    auto responseTree = soap::parseSoapResponse(soapResponse);
    auto [_, messageNode] = responseTree.front();
    return Utils::parseXmlFromString(messageNode.get_value<std::string>());
}

bool xmds::parseSuccessResponse(const std::string& soapResponse)
{
    auto responseTree = soap::parseSoapResponse(soapResponse);
    return responseTree.get_child("success").get_value<bool>();
}

std::string xmds::parseFileResponse(const std::string& soapResponse)
{
    auto responseTree = soap::parseSoapResponse(soapResponse);
    return responseTree.begin()->second.get_value<std::string>();
}

