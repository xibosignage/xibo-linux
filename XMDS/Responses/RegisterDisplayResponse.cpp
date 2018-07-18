#include "RegisterDisplayResponse.hpp"

RegisterDisplayResponse::RegisterDisplayResponse(const std::string& xml_response) :
    XmlResponse(xml_response)
{
    auto attrs = xml_message_tree()->get_child("display").get_child("<xmlattr>");
    m_status = static_cast<Status>(attrs.get<int>("status"));
    m_status_message = attrs.get<std::string>("message");
}

RegisterDisplayResponse::Status RegisterDisplayResponse::status() const
{
    return m_status;
}

const std::string&RegisterDisplayResponse::status_message() const
{
    return m_status_message;
}
