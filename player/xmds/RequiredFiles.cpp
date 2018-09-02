#include "RequiredFiles.hpp"
#include "xmds.hpp"

namespace RequiredFileHelper
{
    auto toFileType(const std::string& type)
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

    auto toDownloadType(const std::string& type)
    {
        using DownloadType = RequiredFiles::Response::DownloadType;

        if(type == "http")
            return DownloadType::HTTP;
        else if(type == "xmds")
            return DownloadType::XMDS;

        return DownloadType::Invalid;
    }
}

const std::vector<RequiredFiles::Response::required_file>&
RequiredFiles::Response::requiredFiles() const
{
    return m_requiredMedia;
}

const std::vector<RequiredFiles::Response::required_resource>&
RequiredFiles::Response::requiredResources() const
{
    return m_requiredResources;
}

void RequiredFiles::Response::addFile(const boost::property_tree::ptree& attrs)
{
    auto fileType = RequiredFileHelper::toFileType(attrs.get<std::string>("type"));
    if(fileType == FileType::Invalid) return;

    if(fileType != FileType::Resource)
    {
        int id = attrs.get<int>("id");
        size_t size = attrs.get<size_t>("size");
        std::string md5 = attrs.get<std::string>("md5");
        std::string path = attrs.get<std::string>("path");
        std::string saveAs = attrs.get<std::string>("saveAs");
        auto downloadType = RequiredFileHelper::toDownloadType(attrs.get<std::string>("download"));
        m_requiredMedia.emplace_back(required_file{id, size, md5, path, saveAs, downloadType, fileType});
    }
    else
    {
        int layoutId = attrs.get<int>("layoutid");
        int regionId = attrs.get<int>("regionid");
        int mediaId = attrs.get<int>("mediaid");
        m_requiredResources.emplace_back(required_resource{layoutId, regionId, mediaId});
    }
}

template<>
std::string soap::requestString(const RequiredFiles::Request& request)
{
    return createRequest<RequiredFiles::Request>(request.serverKey, request.hardwareKey);
}

template<>
RequiredFiles::Response soap::createResponse(const std::string& soapResponse)
{
    auto filesNode = xmds::parseXmlResponse(soapResponse).get_child("files");

    RequiredFiles::Response result;
    std::for_each(filesNode.begin(), filesNode.end(), [&result](const auto& file){
        auto [name, fileNode] = file;
        if(name == "file")
        {
            result.addFile(fileNode.get_child("<xmlattr>"));
        }
    });

    return result;
}
