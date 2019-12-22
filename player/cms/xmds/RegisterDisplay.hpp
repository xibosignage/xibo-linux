#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"
#include "config/PlayerSettings.hpp"

namespace RegisterDisplay
{
    struct Result
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
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> displayName{"displayName"};
        SoapField<std::string> clientType{"clientType"};
        SoapField<std::string> clientVersion{"clientVersion"};
        SoapField<std::string> clientCode{"clientCode"};
        SoapField<std::string> macAddress{"macAddress"};
        SoapField<std::string> xmrChannel{"xmrChannel"};
        SoapField<std::string> xmrPubKey{"xmrPubKey"};
    };
}

template <>
class Soap::RequestSerializer<RegisterDisplay::Request> : public BaseRequestSerializer<RegisterDisplay::Request>
{
public:
    RequestSerializer(const RegisterDisplay::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<RegisterDisplay::Result> : public BaseResponseParser<RegisterDisplay::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RegisterDisplay::Result parseBody(const XmlNode& node) override;
};
