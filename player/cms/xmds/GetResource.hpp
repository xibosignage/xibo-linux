#pragma once

#include "cms/xmds/Soap.hpp"
#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/SoapResponse.hpp"

#include "common/SoapField.hpp"

struct GetResource
{
    class Response : public SoapResponse<Response>
    {
    public:
        std::string resource;

    protected:
        void parseBody(const XmlNode& node) override;
    };

    struct Request : SoapRequest<Request>
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
        SoapField<int> layoutId{"layoutId"};
        SoapField<std::string> regionId{"regionId"};
        SoapField<std::string> mediaId{"mediaId"};
    };
};
