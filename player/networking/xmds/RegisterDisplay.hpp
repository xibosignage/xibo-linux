#pragma once

#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"
#include "Soap.hpp"

#include "common/Field.hpp"
#include "common/settings/PlayerSettings.hpp"

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
    Field<std::string> clientCode{"clientCode"};
    Field<std::string> macAddress{"macAddress"};
    Field<std::string> xmrChannel{"xmrChannel"};
    Field<std::string> xmrPubKey{"xmrPubKey"};
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
    RegisterDisplay::Result parseBody(const ptree_node& node) override;
};
