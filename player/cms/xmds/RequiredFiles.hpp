#pragma once

#include "cms/xmds/BaseRequestSerializer.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"
#include "common/storage/RequiredItems.hpp"

namespace RequiredFiles
{
    struct Result
    {
        const RequiredFilesSet<RegularFile>& requiredFiles() const;
        const RequiredFilesSet<ResourceFile>& requiredResources() const;

        void addFile(RegularFile&& file);
        void addResource(ResourceFile&& resource);

    private:
        RequiredFilesSet<RegularFile> m_requiredFiles;
        RequiredFilesSet<ResourceFile> m_requiredResources;
    };

    struct Request
    {
        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
}

template <>
class Soap::RequestSerializer<RequiredFiles::Request> : public BaseRequestSerializer<RequiredFiles::Request>
{
public:
    RequestSerializer(const RequiredFiles::Request& request);
    std::string string();
};

template <>
class Soap::ResponseParser<RequiredFiles::Result> : public BaseResponseParser<RequiredFiles::Result>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RequiredFiles::Result parseBody(const XmlNode& node) override;

private:
    RegularFile parseRegularFile(const XmlNode& attrs);
    ResourceFile parseResourceFile(const XmlNode& attrs);
    std::pair<std::string, std::string> parseFileNameAndPath(RegularFile::DownloadType dType,
                                                             std::string_view fType,
                                                             const XmlNode& attrs);

    bool isLayout(std::string_view type) const;
    bool isMedia(std::string_view type) const;
    bool isResource(std::string_view type) const;
    RegularFile::DownloadType toDownloadType(std::string_view type);
};
