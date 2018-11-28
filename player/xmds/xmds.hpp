#ifndef XMDS_HPP
#define XMDS_HPP

#include <boost/property_tree/ptree_fwd.hpp>

namespace xmds
{
    boost::property_tree::ptree parseXmlResponse(const std::string& soapResponse);
    bool parseSuccessResponse(const std::string& soapResponse);
    std::string parseFileResponse(const std::string& soapResponse);

}

#endif // XMDS_HPP
