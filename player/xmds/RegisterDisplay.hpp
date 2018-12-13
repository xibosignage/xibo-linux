#ifndef REGISTERDISPLAY_HPP
#define REGISTERDISPLAY_HPP

#include "Field.hpp"
#include "SOAP.hpp"
#include "control/PlayerSettings.hpp"

namespace RegisterDisplay
{
    struct Response
    {
        enum class Status
        {
            Ready,
            Added,
            Waiting,
            Invalid = -1
        };

        Status status = Status::Invalid;
        std::string statusMessage;
        PlayerSettings playerSettings;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<std::string> displayName{"displayName"};
        Field<std::string> clientType{"clientType"};
        Field<std::string> clientVersion{"clientVersion"};
        Field<int> clientCode{"clientCode"};
        Field<std::string> macAddress{"macAddress"};
    };
}

template<>
class SOAP::RequestSerializer<RegisterDisplay::Request> : public BaseRequestSerializer<RegisterDisplay::Request>
{
public:
    RequestSerializer(const RegisterDisplay::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<RegisterDisplay::Response> : public BaseResponseParser
{
public:
    ResponseParser(const std::string& soapResponse);
    RegisterDisplay::Response get();

private:
    void fillPlayerSettings(PlayerSettings& settings, const boost::property_tree::ptree& display);

};


#endif // REGISTERDISPLAY_HPP
