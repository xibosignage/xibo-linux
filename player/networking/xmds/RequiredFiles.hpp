#pragma once

#include "BaseRequestSerializer.hpp"
#include "BaseResponseParser.hpp"
#include "Soap.hpp"

#include "common/Field.hpp"
#include "networking/RequiredItems.hpp"

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
    RequiredFiles::Result parseBody(const ptree_node& node) override;

private:
    RegularFile parseRegularFile(const ptree_node& attrs);
    ResourceFile parseResourceFile(const ptree_node& attrs);
    std::pair<std::string, std::string> parseFileNameAndPath(RegularFile::DownloadType dType, std::string_view fType,
                                                             const ptree_node& attrs);

    bool isLayout(std::string_view type) const;
    bool isMedia(std::string_view type) const;
    bool isResource(std::string_view type) const;
    RegularFile::DownloadType toDownloadType(std::string_view type);
};
