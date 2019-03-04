#pragma once

#include "Field.hpp"
#include "Soap.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

#include "control/PlayerSettings.hpp"

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
class Soap::RequestSerializer<RegisterDisplay::Request> : public BaseRequestSerializer<RegisterDisplay::Request>
{
public:
    RequestSerializer(const RegisterDisplay::Request& request);
    std::string string();

};

template<>
class Soap::ResponseParser<RegisterDisplay::Result> : public BaseResponseParser<RegisterDisplay::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RegisterDisplay::Result doParse(const xml_node& node) override;

private:
    void fillPlayerSettings(PlayerSettings& settings, const xml_node& display);
    LoggingLevel toLogLevelEnum(const std::string& level);

};
