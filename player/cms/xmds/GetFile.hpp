#pragma once

#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"

struct GetFile
{
    struct Response
    {
        std::string base64chunk;
    };

    struct Request : SoapRequest
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> fileId{"fileId"};
        SoapField<std::string> fileType{"fileType"};
        SoapField<std::size_t> chunkOffset{"chunkOffset"};
        SoapField<std::size_t> chunkSize{"chuckSize"};
    };
};

template <>
class Soap::ResponseParser<GetFile::Response> : public BaseResponseParser<GetFile::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    GetFile::Response parseBody(const XmlNode& node) override;
};
