#include "RequiredFiles.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::RequiredFiles;

const RequiredFilesSet<RegularFile>& RequiredFiles::Result::requiredFiles() const
{
    return m_requiredFiles;
}

const RequiredFilesSet<ResourceFile>& RequiredFiles::Result::requiredResources() const
{
    return m_requiredResources;
}

void RequiredFiles::Result::addFile(RegularFile&& file)
{
    m_requiredFiles.emplace_back(std::move(file));
}

void RequiredFiles::Result::addResource(ResourceFile&& resource)
{
    m_requiredResources.emplace_back(std::move(resource));
}

Soap::RequestSerializer<RequiredFiles::Request>::RequestSerializer(const RequiredFiles::Request& request) :
    BaseRequestSerializer(request)
{
}

std::string Soap::RequestSerializer<RequiredFiles::Request>::string()
{
    return createRequest(Resources::Name, request().serverKey, request().hardwareKey);
}

Soap::ResponseParser<RequiredFiles::Result>::ResponseParser(const std::string& soapResponse) :
    BaseResponseParser(soapResponse)
{
}

RequiredFiles::Result Soap::ResponseParser<RequiredFiles::Result>::parseBody(const XmlNode& node)
{
    auto requiredFilesXml = node.get<std::string>(Resources::RequiredFilesXml);
    auto filesNode = Parsing::xmlFrom(requiredFilesXml).get_child(Resources::Files);

    RequiredFiles::Result result;

    for (auto [name, fileNode] : filesNode)
    {
        if (name != Resources::File) continue;

        auto fileAttrs = fileNode.get_child(Resources::FileAttrs);
        auto fileType = fileAttrs.get<std::string>(Resources::FileType);

        if (isLayout(fileType) || isMedia(fileType))
        {
            result.addFile(parseRegularFile(fileAttrs));
        }
        else if (isResource(fileType))
        {
            result.addResource(parseResourceFile(fileAttrs));
        }
    }

    return result;
}

RegularFile Soap::ResponseParser<RequiredFiles::Result>::parseRegularFile(const XmlNode& attrs)
{
    auto fileType = attrs.get<std::string>(Resources::FileType);
    auto id = attrs.get<int>(Resources::RegularFile::Id);
    auto size = attrs.get<size_t>(Resources::RegularFile::Size);
    auto md5 = Md5Hash{attrs.get<std::string>(Resources::RegularFile::MD5)};
    auto downloadType = toDownloadType(attrs.get<std::string>(Resources::RegularFile::DownloadType));
    auto [path, name] = parseFileNameAndPath(downloadType, fileType, attrs);

    return RegularFile{id, size, md5, path, name, fileType, downloadType};
}

ResourceFile Soap::ResponseParser<RequiredFiles::Result>::parseResourceFile(const XmlNode& attrs)
{
    auto layoutId = attrs.get<int>(Resources::ResourceFile::MediaId);
    auto regionId = attrs.get<int>(Resources::ResourceFile::RegionId);
    auto mediaId = attrs.get<int>(Resources::ResourceFile::MediaId);
    auto lastUpdate = DateTime::utcFromTimestamp(attrs.get<int>(Resources::ResourceFile::LastUpdate));

    return ResourceFile{layoutId, regionId, mediaId, lastUpdate};
}

bool Soap::ResponseParser<RequiredFiles::Result>::isLayout(std::string_view type) const
{
    return type == Resources::LayoutType;
}

bool Soap::ResponseParser<RequiredFiles::Result>::isMedia(std::string_view type) const
{
    return type == Resources::MediaType;
}

bool Soap::ResponseParser<RequiredFiles::Result>::isResource(std::string_view type) const
{
    return type == Resources::ResourceType;
}

RegularFile::DownloadType Soap::ResponseParser<RequiredFiles::Result>::toDownloadType(std::string_view type)
{
    if (type == Resources::RegularFile::HttpDownload)
        return RegularFile::DownloadType::HTTP;
    else if (type == Resources::RegularFile::XmdsDownload)
        return RegularFile::DownloadType::XMDS;

    return RegularFile::DownloadType::Invalid;
}

// NOTE: workaround because filePath and fileName from RequiredFiles request are a bit clumsy to parse directly
std::pair<std::string, std::string> Soap::ResponseParser<RequiredFiles::Result>::parseFileNameAndPath(
    RegularFile::DownloadType dType,
    std::string_view fType,
    const XmlNode& attrs)
{
    std::string path, name;

    switch (dType)
    {
        case RegularFile::DownloadType::HTTP:
            path = attrs.get<std::string>(Resources::RegularFile::Path);
            name = attrs.get<std::string>(Resources::RegularFile::Name);
            break;
        case RegularFile::DownloadType::XMDS:
            name = attrs.get<std::string>(Resources::RegularFile::Path);
            if (isLayout(fType))
            {
                name += ".xlf";
            }
            break;
        default: break;
    }

    return std::pair{path, name};
}
