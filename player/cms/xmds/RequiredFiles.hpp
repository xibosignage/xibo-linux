#pragma once

#include "cms/xmds/Soap.hpp"
#include "cms/xmds/SoapRequest.hpp"
#include "cms/xmds/SoapResponse.hpp"

#include "common/SoapField.hpp"
#include "networking/RequiredItems.hpp"

struct RequiredFiles
{
    class Response : public SoapResponse<Response>
    {
    public:
        const FilesToDownload<RegularFile>& requiredFiles() const;
        const FilesToDownload<ResourceFile>& requiredResources() const;

        void addFile(RegularFile&& file);
        void addResource(ResourceFile&& resource);

    protected:
        void parseBody(const XmlNode& node) override;

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

    private:
        FilesToDownload<RegularFile> requiredFiles_;
        FilesToDownload<ResourceFile> requiredResources_;
    };

    struct Request : SoapRequest<Request>
    {
        std::string string() const override;

        SoapField<std::string> serverKey{"serverKey"};
        SoapField<std::string> hardwareKey{"hardwareKey"};
    };
};
