#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

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

template<>
class SOAP::RequestSerializer<GetFile::Request> : public BaseRequestSerializer<GetFile::Request>
{
public:
    RequestSerializer(const GetFile::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<GetFile::Result> : public BaseResponseParser<GetFile::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    GetFile::Result doParse(const xml_node& node) override;

};
