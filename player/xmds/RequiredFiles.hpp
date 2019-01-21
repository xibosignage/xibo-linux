#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

namespace RequiredFiles
{
    struct Result
    {
        enum class FileType
        {
            Media,
            Layout,
            Resource,
            Invalid
        };

        enum class DownloadType
        {
            HTTP,
            XMDS,
            Invalid
        };

        struct RequiredFile
        {
            int id;
            size_t size;
            std::string md5;
            std::string path;
            std::string name;
            DownloadType downloadType;
            FileType fileType;
        };

        struct RequiredResource
        {
            int layoutId;
            int regionId;
            int mediaId;
        };

        std::vector<RequiredFile> requiredFiles;
        std::vector<RequiredResource> requiredResources;

    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

using RegularFiles = std::vector<RequiredFiles::Result::RequiredFile>;
using ResourceFiles = std::vector<RequiredFiles::Result::RequiredResource>;

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
    RequiredFiles::Result::FileType toFileType(const std::string& type);
    RequiredFiles::Result::DownloadType toDownloadType(const std::string& type);

};
