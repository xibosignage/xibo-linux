#pragma once

#include "cms/xmds/Soap.hpp"
#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/SoapResponse.hpp"

#include "common/SoapField.hpp"

struct SubmitStats
{
    class Response : public SoapResponse<Response>
    {
    public:
        bool success;

    protected:
        void parseBody(const XmlNode& node) override;
    };

    struct Request : SoapRequest<Request>
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<std::string> statXml{"statXml"};
    };
};
