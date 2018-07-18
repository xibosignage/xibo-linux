#ifndef REGISTERDISPLAYRESPONSE_HPP
#define REGISTERDISPLAYRESPONSE_HPP

#include "XmlResponse.hpp"
#include "constants.hpp"

class RegisterDisplayResponse : public XmlResponse<REGISTER_DISPLAY>
{
public:
    enum class Status
    {
        READY,
        ADDED,
        WAITING
    };

    RegisterDisplayResponse(const std::string& xml_response);
    Status status() const;
    const std::string& status_message() const;

private:
    Status m_status;
    std::string m_status_message;

};

#endif // REGISTERDISPLAYRESPONSE_HPP
