#include "RequiredFiles.hpp"

#include "cms/xmds/Resources.hpp"

namespace Resources = XmdsResources::RequiredFiles;

const FilesToDownload<RegularFile>& RequiredFiles::Response::requiredFiles() const
{
    return requiredFiles_;
}

const FilesToDownload<ResourceFile>& RequiredFiles::Response::requiredResources() const
{
    return requiredResources_;
}

void RequiredFiles::Response::addFile(RegularFile&& file)
{
    requiredFiles_.emplace_back(std::move(file));
}

void RequiredFiles::Response::addResource(ResourceFile&& resource)
{
    requiredResources_.emplace_back(std::move(resource));
}

void RequiredFiles::Response::parseBody(const XmlNode& node)
{
    auto requiredFilesXml = node.get<std::string>(Resources::RequiredFilesXml);
    auto filesNode = Parsing::xmlFrom(requiredFilesXml).get_child(Resources::Files);

    for (auto [name, fileNode] : filesNode)
    {
        if (name != Resources::File) continue;

        auto fileAttrs = fileNode.get_child(Resources::FileAttrs);
        auto fileType = fileAttrs.get<std::string>(Resources::FileType);

        if (isLayout(fileType) || isMedia(fileType))
        {
            addFile(parseRegularFile(fileAttrs));
        }
        else if (isResource(fileType))
        {
            addResource(parseResourceFile(fileAttrs));
        }
    }
}

RegularFile RequiredFiles::Response::parseRegularFile(const XmlNode& attrs)
{
    auto fileType = attrs.get<std::string>(Resources::FileType);
    auto id = attrs.get<int>(Resources::RegularFile::Id);
    auto size = attrs.get<size_t>(Resources::RegularFile::Size);
    auto md5 = attrs.get<std::string>(Resources::RegularFile::MD5);
    auto downloadType = toDownloadType(attrs.get<std::string>(Resources::RegularFile::DownloadType));
    auto [path, name] = parseFileNameAndPath(downloadType, fileType, attrs);

    return RegularFile{id, size, md5, path, name, fileType, downloadType};
}

ResourceFile RequiredFiles::Response::parseResourceFile(const XmlNode& attrs)
{
    auto layoutId = attrs.get<int>(Resources::ResourceFile::MediaId);
    auto regionId = attrs.get<int>(Resources::ResourceFile::RegionId);
    auto mediaId = attrs.get<int>(Resources::ResourceFile::MediaId);

    return ResourceFile{layoutId, regionId, mediaId};
}

bool RequiredFiles::Response::isLayout(std::string_view type) const
{
    return type == Resources::LayoutType;
}

bool RequiredFiles::Response::isMedia(std::string_view type) const
{
    return type == Resources::MediaType;
}

bool RequiredFiles::Response::isResource(std::string_view type) const
{
    return type == Resources::ResourceType;
}

RegularFile::DownloadType RequiredFiles::Response::toDownloadType(std::string_view type)
{
    if (type == Resources::RegularFile::HttpDownload)
        return RegularFile::DownloadType::HTTP;
    else if (type == Resources::RegularFile::XmdsDownload)
        return RegularFile::DownloadType::XMDS;

    return RegularFile::DownloadType::Invalid;
}

// NOTE: workaround because filePath and fileName from RequiredFiles request are a bit clumsy to parse directly
std::pair<std::string, std::string> RequiredFiles::Response::parseFileNameAndPath(RegularFile::DownloadType dType,
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

std::string RequiredFiles::Request::string() const
{
    return toSoapString(Resources::Name, serverKey, hardwareKey);
}
