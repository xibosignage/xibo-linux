#ifndef REGISTERDISPLAYREQUEST_HPP
#define REGISTERDISPLAYREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class RegisterDisplayRequest : public BaseRequest<REGISTER_DISPLAY, std::string, std::string, std::string, int, std::string>
{
public:
    using ResponseType = RegisterDisplayResponse;

    RegisterDisplayRequest();
    std::string display_name() const;
    void display_name(const std::string& value);
    std::string client_type() const;
    void client_type(const std::string& value);
    std::string client_version() const;
    void client_version(const std::string& value);
    int client_code() const;
    void client_code(int value);
    std::string mac_address() const;
    void mac_address(const std::string& value);
};

#endif // REGISTERDISPLAYREQUEST_HPP
