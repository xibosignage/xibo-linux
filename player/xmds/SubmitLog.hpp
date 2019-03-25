#pragma once

#include "Soap.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

#include "utils/Field.hpp"

namespace SubmitLog
{
    struct Result
    {
        bool success;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<std::string> logXml{"logXml"};
    };
}

template<>
class Soap::RequestSerializer<SubmitLog::Request> : public BaseRequestSerializer<SubmitLog::Request>
{
public:
    RequestSerializer(const SubmitLog::Request& request);
    std::string string();

};

template<>
class Soap::ResponseParser<SubmitLog::Result> : public BaseResponseParser<SubmitLog::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    SubmitLog::Result doParse(const xml_node& node) override;

};

