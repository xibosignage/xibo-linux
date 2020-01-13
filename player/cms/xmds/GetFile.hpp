#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

namespace GetFile
{
    struct Result
    {
        std::string base64chunk;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> fileId{"fileId"};
        SoapField<std::string> fileType{"fileType"};
        SoapField<std::size_t> chunkOffset{"chunkOffset"};
        SoapField<std::size_t> chunkSize{"chuckSize"};
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
    GetFile::Result parseBody(const XmlNode& node) override;
};
