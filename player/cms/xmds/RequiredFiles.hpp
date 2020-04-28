#pragma once

#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/BaseResponseParser.hpp"
#include "cms/xmds/Soap.hpp"

#include "common/SoapField.hpp"
#include "networking/RequiredItems.hpp"

struct RequiredFiles
{
    struct Response
    {
        const FilesToDownload<RegularFile>& requiredFiles() const;
        const FilesToDownload<ResourceFile>& requiredResources() const;

        void addFile(RegularFile&& file);
        void addResource(ResourceFile&& resource);

    private:
        FilesToDownload<RegularFile> m_requiredFiles;
        FilesToDownload<ResourceFile> m_requiredResources;
    };

    struct Request : SoapRequest
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
};

template <>
class Soap::ResponseParser<RequiredFiles::Response> : public BaseResponseParser<RequiredFiles::Response>
{
public:
    ResponseParser(const std::string& soapResponse);

protected:
    RequiredFiles::Response parseBody(const XmlNode& node) override;

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
