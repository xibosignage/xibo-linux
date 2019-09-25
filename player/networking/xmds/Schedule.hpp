#pragma once

#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"
#include "Soap.hpp"

#include "common/Field.hpp"

namespace Schedule
{
struct Result
{
    std::string scheduleXml;
};

struct Request
{
    Field<std::string> serverKey{"serverKey"};
    Field<std::string> hardwareKey{"hardwareKey"};
};
}

template <>
class Soap::RequestSerializer<Schedule::Request> : public BaseRequestSerializer<Schedule::Request>
{
public:
    RequestSerializer(const Schedule::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<Schedule::Result> : public BaseResponseParser<Schedule::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    Schedule::Result parseBody(const ptree_node& scheduleNode) override;
};
