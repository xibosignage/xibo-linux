#pragma once

#include "Field.hpp"
#include "SOAP.hpp"
#include "BaseResponseParser.hpp"
#include "BaseRequestSerializer.hpp"

#include "managers/RequiredItems.hpp"

namespace RequiredFiles
{
    struct Result
    {
        const FilesToDownload<RegularFile>& requiredFiles() const;
        const FilesToDownload<ResourceFile>& requiredResources() const;

        void addFile(RegularFile&& file);
        void addResource(ResourceFile&& resource);

    private:
        FilesToDownload<RegularFile> m_requiredFiles;
        FilesToDownload<ResourceFile> m_requiredResources;
    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

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
    RequiredFiles::Result doParse(const xml_node& node) override;

private:
    RegularFile parseRegularFile(const xml_node& attrs);
    ResourceFile parseResourceFile(const xml_node& attrs);
    std::pair<std::string, std::string> parseFileNameAndPath(DownloadType dType, std::string_view fType, const xml_node& attrs);

    bool isLayout(std::string_view type) const;
    bool isMedia(std::string_view type) const;
    bool isResource(std::string_view type) const;
    DownloadType toDownloadType(std::string_view type);

};
