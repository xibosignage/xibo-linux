#pragma once

#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"
#include "cms/xmds/SoapRequest.hpp"

#include "common/SoapField.hpp"
#include "config/PlayerSettings.hpp"

struct RegisterDisplay
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

    struct Request : SoapRequest
    {
        std::string string() const override;

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
};

template <>
class Soap::ResponseParser<RegisterDisplay::Response> : public BaseResponseParser<RegisterDisplay::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RegisterDisplay::Response parseBody(const XmlNode& node) override;
};
