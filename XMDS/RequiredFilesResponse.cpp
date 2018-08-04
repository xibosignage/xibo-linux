#include "RequiredFilesResponse.hpp"

namespace RequiredFileHelper
{
    auto to_file_type(const std::string& type)
    {
        using FileType = RequiredFilesResponse::RequiredFile::FileType;

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
        using DownloadType = RequiredFilesResponse::RequiredFile::DownloadType;

        if(type == "http")
            return DownloadType::HTTP;
        else if(type == "xmds")
            return DownloadType::XMDS;

        return DownloadType::Invalid;
    }
}

RequiredFilesResponse::RequiredFilesResponse(const std::string& xml_response) :
    XmlResponse(xml_response)
{
    if(xml_message_tree())
    {
        auto files_node = xml_message_tree()->get_child("files");
        std::for_each(files_node.begin(), files_node.end(), [=](const auto& file){
            auto [name, file_node] = file;
            if(name == "file")
            {
                add_file(file_node.get_child("<xmlattr>"));
            }
        });
    }
}

const std::vector<RequiredFilesResponse::RequiredFile>& RequiredFilesResponse::required_files() const
{
    return m_required_files;
}

void RequiredFilesResponse::add_file(const boost::property_tree::ptree& attrs)
{
    auto file_type = RequiredFileHelper::to_file_type(attrs.get<std::string>("type"));
    if(file_type != RequiredFile::FileType::Invalid)
    {
        int id = attrs.get<int>("id");
        size_t size = attrs.get<size_t>("size");
        std::string md5 = attrs.get<std::string>("md5");
        std::string path = attrs.get<std::string>("path");
        std::string save_as = attrs.get<std::string>("saveAs");
        auto download_type = RequiredFileHelper::to_download_type(attrs.get<std::string>("download"));
        m_required_files.emplace_back(RequiredFile{file_type, id, size, md5, path, save_as, download_type});
    }
}
