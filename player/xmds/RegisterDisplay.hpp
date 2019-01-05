#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

#include "control/PlayerSettings.hpp"

namespace RegisterDisplay
{
    struct Response
    {
        struct Status
        {
            enum class Code
            {
                Ready,
                Added,
                Waiting,
                Invalid = -1
            };

            Code code = Code::Invalid;
            std::string message;
        };

        Status status;
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
class SOAP::ResponseParser<RegisterDisplay::Response> : public BaseResponseParser<RegisterDisplay::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RegisterDisplay::Response doParse(const boost::property_tree::ptree& node) override;

private:
    void fillPlayerSettings(PlayerSettings& settings, const boost::property_tree::ptree& display);
};
