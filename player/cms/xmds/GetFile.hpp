#pragma once

#include "cms/xmds/Soap.hpp"
#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/SoapResponse.hpp"

#include "common/SoapField.hpp"

struct GetFile
{
    class Response : public SoapResponse<Response>
    {
    public:
        std::string base64chunk;

    protected:
        void parseBody(const XmlNode& node) override;
    };

    struct Request : SoapRequest<Request>
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
