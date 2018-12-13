#include "RequiredFiles.hpp"

#include "utils/Utilities.hpp"

constexpr const std::string_view REQUEST_NAME = "RequiredFiles";

SOAP::RequestSerializer<RequiredFiles::Request>::RequestSerializer(const RequiredFiles::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<RequiredFiles::Request>::string()
{
    return createRequest(REQUEST_NAME, request().serverKey, request().hardwareKey);
}

SOAP::ResponseParser<RequiredFiles::Response>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

RequiredFiles::Response SOAP::ResponseParser<RequiredFiles::Response>::get()
{
    auto requiredFilesXml = responseTree().get_child("RequiredFilesXml").get_value<std::string>();
    auto filesNode = Utils::parseXmlFromString(requiredFilesXml).get_child("files");

    RequiredFiles::Response result;
    std::for_each(filesNode.begin(), filesNode.end(), [this, &result](const auto& file){
        auto [name, fileNode] = file;
        if(name == "file")
        {
            addRequiredFile(result, fileNode.get_child("<xmlattr>"));
        }
    });

    return result;
}

void SOAP::ResponseParser<RequiredFiles::Response>::addRequiredFile(RequiredFiles::Response& response, const boost::property_tree::ptree& attrs)
{
    using FileType = RequiredFiles::Response::FileType;

    auto fileType = toFileType(attrs.get<std::string>("type"));
    if(fileType == FileType::Invalid) return;

    if(fileType != FileType::Resource)
    {
        int id = attrs.get<int>("id");
        size_t size = attrs.get<size_t>("size");
        std::string md5 = attrs.get<std::string>("md5");
        std::string path = attrs.get<std::string>("path");
        std::string saveAs = attrs.get<std::string>("saveAs");
        auto downloadType = toDownloadType(attrs.get<std::string>("download"));
        response.requiredFiles.emplace_back(RequiredFiles::Response::RequiredFile{id, size, md5, path, saveAs, downloadType, fileType});
    }
    else
    {
        int layoutId = attrs.get<int>("layoutid");
        int regionId = attrs.get<int>("regionid");
        int mediaId = attrs.get<int>("mediaid");
        response.requiredResources.emplace_back(RequiredFiles::Response::RequiredResource{layoutId, regionId, mediaId});
    }
}

RequiredFiles::Response::FileType SOAP::ResponseParser<RequiredFiles::Response>::toFileType(const std::string& type)
{
    using FileType = RequiredFiles::Response::FileType;

    if(type == "media")
        return FileType::Media;
    else if(type == "layout")
        return FileType::Layout;
    else if(type == "resource")
        return FileType::Resource;

    return FileType::Invalid;
}

RequiredFiles::Response::DownloadType SOAP::ResponseParser<RequiredFiles::Response>::toDownloadType(const std::string& type)
{
    using DownloadType = RequiredFiles::Response::DownloadType;

    if(type == "http")
        return DownloadType::HTTP;
    else if(type == "xmds")
        return DownloadType::XMDS;

    return DownloadType::Invalid;
}
