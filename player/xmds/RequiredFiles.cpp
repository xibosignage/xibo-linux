#include "RequiredFiles.hpp"
#include "xmds.hpp"

namespace RequiredFileHelper
{
    auto to_file_type(const std::string& type)
    {
        using FileType = RequiredFiles::response::required_file::FileType;

        if(type == "media")
            return FileType::Media;
        else if(type == "layout")
            return FileType::Layout;
        else if(type == "resourse")
            return FileType::Resourse;

        return FileType::Invalid;
    }

    auto to_download_type(const std::string& type)
    {
        using DownloadType = RequiredFiles::response::required_file::DownloadType;

        if(type == "http")
            return DownloadType::HTTP;
        else if(type == "xmds")
            return DownloadType::XMDS;

        return DownloadType::Invalid;
    }
}

const std::vector<RequiredFiles::response::required_file>&
RequiredFiles::response::required_files() const
{
    return m_required_files;
}

void RequiredFiles::response::add_file(const boost::property_tree::ptree& attrs)
{
    auto file_type = RequiredFileHelper::to_file_type(attrs.get<std::string>("type"));
    if(file_type != required_file::FileType::Invalid)
    {
        int id = attrs.get<int>("id");
        size_t size = attrs.get<size_t>("size");
        std::string md5 = attrs.get<std::string>("md5");
        std::string path = attrs.get<std::string>("path");
        std::string save_as = attrs.get<std::string>("saveAs");
        auto download_type = RequiredFileHelper::to_download_type(attrs.get<std::string>("download"));
        m_required_files.emplace_back(required_file{file_type, id, size, md5, path, save_as, download_type});
    }
}

template<>
std::string soap::request_string(const RequiredFiles::request& request)
{
    return create_request<RequiredFiles::request>(request.server_key, request.hardware_key);
}

template<>
RequiredFiles::response soap::create_response(const std::string& soap_response)
{
    auto files_node = xmds::parse_xml_response(soap_response).get_child("files");

    RequiredFiles::response result;
    std::for_each(files_node.begin(), files_node.end(), [&result](const auto& file){
        auto [name, file_node] = file;
        if(name == "file")
        {
            result.add_file(file_node.get_child("<xmlattr>"));
        }
    });

    return result;
}
