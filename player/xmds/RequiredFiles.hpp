#ifndef REQUIREDFILES_HPP
#define REQUIREDFILES_HPP

#include "Field.hpp"
#include "SOAP.hpp"


namespace RequiredFiles
{
    struct Response
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
            std::string filename;
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

using RegularFiles = std::vector<RequiredFiles::Response::RequiredFile>;
using ResourceFiles = std::vector<RequiredFiles::Response::RequiredResource>;

template<>
class SOAP::RequestSerializer<RequiredFiles::Request> : public BaseRequestSerializer<RequiredFiles::Request>
{
public:
    RequestSerializer(const RequiredFiles::Request& request);
    std::string string();

};

template<>
class SOAP::ResponseParser<RequiredFiles::Response> : public BaseResponseParser
{
public:
    ResponseParser(const std::string& soapResponse);
    RequiredFiles::Response get();

private:
    void addRequiredFile(RequiredFiles::Response& response, const boost::property_tree::ptree& attrs);
    RequiredFiles::Response::FileType toFileType(const std::string& type);
    RequiredFiles::Response::DownloadType toDownloadType(const std::string& type);

};


#endif // REQUIREDFILES_HPP
