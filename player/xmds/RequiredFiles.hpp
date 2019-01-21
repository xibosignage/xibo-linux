#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

#include "managers/RequiredItems.hpp"

namespace RequiredFiles
{
    struct Result
    {
        RegularFiles requiredFiles;
        ResourceFiles requiredResources;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

template<>
class SOAP::RequestSerializer<RequiredFiles::Request> : public BaseRequestSerializer<RequiredFiles::Request>
{
public:
    RequestSerializer(const RequiredFiles::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<RequiredFiles::Result> : public BaseResponseParser<RequiredFiles::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RequiredFiles::Result doParse(const boost::property_tree::ptree& node) override;

private:
    void addRequiredFile(RequiredFiles::Result& response, const boost::property_tree::ptree& attrs);
    bool isLayoutOrMedia(const std::string& type) const;
    bool isResource(const std::string& type) const;
    DownloadType toDownloadType(const std::string& type);

};
