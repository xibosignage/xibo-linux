#pragma once

#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"
#include "Soap.hpp"

#include "common/Field.hpp"

namespace GetFile
{
struct Result
{
    std::string base64chunk;
};

struct Request
{
    Field<std::string> serverKey{"serverKey"};
    Field<std::string> hardwareKey{"hardwareKey"};
    Field<std::string> fileId{"fileId"};
    Field<std::string> fileType{"fileType"};
    Field<std::size_t> chunkOffset{"chunkOffset"};
    Field<std::size_t> chunkSize{"chuckSize"};
};
}

template <>
class Soap::RequestSerializer<GetFile::Request> : public BaseRequestSerializer<GetFile::Request>
{
public:
    RequestSerializer(const GetFile::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<GetFile::Result> : public BaseResponseParser<GetFile::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    GetFile::Result parseBody(const ptree_node& node) override;
};
