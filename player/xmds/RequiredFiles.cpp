#include "RequiredFiles.hpp"
#include "xmds.hpp"

namespace RequiredFileHelper
{
    auto to_file_type(const std::string& type)
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

    auto to_download_type(const std::string& type)
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
RequiredFiles::Response::required_files() const
{
    return m_required_media;
}

const std::vector<RequiredFiles::Response::required_resource>&
RequiredFiles::Response::required_resources() const
{
    return m_required_resources;
}

void RequiredFiles::Response::add_file(const boost::property_tree::ptree& attrs)
{
    auto file_type = RequiredFileHelper::to_file_type(attrs.get<std::string>("type"));
    if(file_type == FileType::Invalid) return;

    if(file_type != FileType::Resource)
    {
        int id = attrs.get<int>("id");
        size_t size = attrs.get<size_t>("size");
        std::string md5 = attrs.get<std::string>("md5");
        std::string path = attrs.get<std::string>("path");
        std::string save_as = attrs.get<std::string>("saveAs");
        auto download_type = RequiredFileHelper::to_download_type(attrs.get<std::string>("download"));
        m_required_media.emplace_back(required_file{id, size, md5, path, save_as, download_type, file_type});
    }
    else
    {
        int layout_id = attrs.get<int>("layoutid");
        int region_id = attrs.get<int>("regionid");
        int media_id = attrs.get<int>("mediaid");
        m_required_resources.emplace_back(required_resource{layout_id, region_id, media_id});
    }
}

template<>
std::string soap::request_string(const RequiredFiles::Request& request)
{
    return create_request<RequiredFiles::Request>(request.server_key, request.hardware_key);
}

template<>
RequiredFiles::Response soap::create_response(const std::string& soap_response)
{
    auto files_node = xmds::parse_xml_response(soap_response).get_child("files");

    RequiredFiles::Response result;
    std::for_each(files_node.begin(), files_node.end(), [&result](const auto& file){
        auto [name, file_node] = file;
        if(name == "file")
        {
            result.add_file(file_node.get_child("<xmlattr>"));
        }
    });

    return result;
}
