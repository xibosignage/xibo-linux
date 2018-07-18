#ifndef XMLRESPONSE_HPP
#define XMLRESPONSE_HPP

#include "BaseResponse.hpp"

template<const char* ResponseName>
class XmlResponse : public BaseResponse<ResponseName>
{
public:
    XmlResponse(const std::string& xml_response) :
        BaseResponse<ResponseName>(xml_response)
    {
        auto [_, message_node] = *this->response_tree()->begin();
        std::cout << message_node.template get_value<std::string>() << std::endl;
        m_xml_message_tree = utils::parse_xml(message_node.template get_value<std::string>());
    }

protected:
    boost::optional<boost::property_tree::ptree> xml_message_tree() const
    {
        return m_xml_message_tree;
    }

private:
    boost::optional<boost::property_tree::ptree> m_xml_message_tree;

};

#endif // XMLRESPONSE_HPP
