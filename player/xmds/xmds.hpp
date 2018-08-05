#ifndef XMDS_HPP
#define XMDS_HPP

#include <boost/property_tree/ptree.hpp>

namespace xmds
{
    boost::property_tree::ptree parse_xml_response(const std::string& soap_response);
    bool parse_success_response(const std::string& soap_response);
    std::string parse_file_response(const std::string& soap_response);

}

#endif // XMDS_HPP
