#include "RequiredFiles.hpp"

#include "Resources.hpp"
#include "utils/Utilities.hpp"

namespace Resources = XMDSResources::RequiredFiles;

SOAP::RequestSerializer<RequiredFiles::Request>::RequestSerializer(const RequiredFiles::Request& request) : BaseRequestSerializer(request)
{
}

std::string SOAP::RequestSerializer<RequiredFiles::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey);
}

SOAP::ResponseParser<RequiredFiles::Result>::ResponseParser(const std::string& soapResponse) : BaseResponseParser(soapResponse)
{
}

RequiredFiles::Result SOAP::ResponseParser<RequiredFiles::Result>::doParse(const boost::property_tree::ptree& node)
{
    auto requiredFilesXml = node.get<std::string>(Resources::RequiredFilesXml);
    auto filesNode = Utils::parseXmlFromString(requiredFilesXml).get_child(Resources::Files);

    RequiredFiles::Result result;
    std::for_each(filesNode.begin(), filesNode.end(), [this, &result](const auto& file){
        auto [name, fileNode] = file;
        if(name == Resources::File)
        {
            addRequiredFile(result, fileNode.get_child(Resources::FileAttrs));
        }
    });

    return result;
}

void SOAP::ResponseParser<RequiredFiles::Result>::addRequiredFile(RequiredFiles::Result& response, const boost::property_tree::ptree& attrs)
{
    auto fileType = attrs.get<std::string>(Resources::FileType);

    if(isLayoutOrMedia(fileType))
    {
        auto id = attrs.get<int>(Resources::RegularFile::Id);
        auto size = attrs.get<size_t>(Resources::RegularFile::Size);
        auto md5 = attrs.get<std::string>(Resources::RegularFile::MD5);
        auto path = attrs.get<std::string>(Resources::RegularFile::Path);
        auto saveAs = attrs.get_optional<std::string>(Resources::RegularFile::SaveAs);
        auto downloadType = toDownloadType(attrs.get<std::string>(Resources::RegularFile::DownloadType));
        response.requiredFiles.emplace_back(RequiredFile{id, size, md5, path, saveAs.value_or(std::string{}), fileType, downloadType});
    }
    else if(isResource(fileType))
    {
        auto layoutId = attrs.get<int>(Resources::ResourceFile::MediaId);
        auto regionId = attrs.get<int>(Resources::ResourceFile::RegionId);
        auto mediaId = attrs.get<int>(Resources::ResourceFile::MediaId);
        response.requiredResources.emplace_back(RequiredResource{layoutId, regionId, mediaId});
    }
}

bool SOAP::ResponseParser<RequiredFiles::Result>::isLayoutOrMedia(const std::string& type) const
{
    return type == Resources::MediaType || type == Resources::LayoutType;
}

bool SOAP::ResponseParser<RequiredFiles::Result>::isResource(const std::string& type) const
{
    return type == Resources::ResourceType;
}

DownloadType SOAP::ResponseParser<RequiredFiles::Result>::toDownloadType(const std::string& type)
{
    if(type == Resources::RegularFile::HTTPDownload)
        return DownloadType::HTTP;
    else if(type == Resources::RegularFile::XMDSDownload)
        return DownloadType::XMDS;

    return DownloadType::Invalid;
}
