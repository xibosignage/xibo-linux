#pragma once

#include "cms/xmds/Soap.hpp"
#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/SoapResponse.hpp"

#include "common/SoapField.hpp"

struct Schedule
{
    class Response : public SoapResponse<Response>
    {
    public:
        std::string scheduleXml;

    protected:
        void parseBody(const XmlNode& node) override;
    };

    struct Request : SoapRequest<Request>
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
};
