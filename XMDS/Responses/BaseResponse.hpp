#ifndef BASERESPONSE_HPP
#define BASERESPONSE_HPP

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

#include "utils.hpp"

template<const char* ResponseName>
class BaseResponse
{
public:
    BaseResponse(const std::string& xml_response)
    {
        std::string full_response_name = "ns1:" + std::string(ResponseName) + "Response";
        m_response_tree = utils::parse_xml(xml_response).get_child("SOAP-ENV:Envelope")
                                                        .get_child("SOAP-ENV:Body")
                                                        .get_child_optional(full_response_name);
        if(!m_response_tree)
        {
            std::cout << "Fault" << std::endl;
        }
    }

protected:
    boost::optional<boost::property_tree::ptree> response_tree() const
    {
        return m_response_tree;
    }

private:
    boost::optional<boost::property_tree::ptree> m_response_tree;

};

#endif // BASERESPONSE_HPP
