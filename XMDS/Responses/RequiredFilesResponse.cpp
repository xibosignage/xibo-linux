#include "RequiredFilesResponse.hpp"

RequiredFilesResponse::RequiredFile::RequiredFile(const std::string& type,
                                                  int id,
                                                  size_t size,
                                                  const std::string& md5,
                                                  const std::string& path,
                                                  const std::string& filename,
                                                  const std::string& download) :
    m_type(type), m_id(id), m_size(size), m_md5(md5), m_path(path), m_filename(filename), m_download(download)
{
}

const std::string& RequiredFilesResponse::RequiredFile::type() const
{
    return m_type;
}

int RequiredFilesResponse::RequiredFile::id() const
{
    return m_id;
}

size_t RequiredFilesResponse::RequiredFile::size() const
{
    return m_size;
}

const std::string& RequiredFilesResponse::RequiredFile::md5() const
{
    return m_md5;
}

const std::string& RequiredFilesResponse::RequiredFile::path() const
{
    return m_path;
}

const std::string& RequiredFilesResponse::RequiredFile::filename() const
{
    return m_filename;
}

const std::string& RequiredFilesResponse::RequiredFile::download() const
{
    return m_download;
}

RequiredFilesResponse::RequiredFilesResponse(const std::string& xml_response) :
    XmlResponse(xml_response)
{
    auto files_node = xml_message_tree()->get_child("files");
    for(auto [name, file_node] : files_node)
    {
        if(name == "file")
        {
            auto attrs = file_node.get_child("<xmlattr>");
            std::string type = attrs.get<std::string>("type");
            if(type == "media")
            {
                int id = attrs.get<int>("id");
                size_t size = attrs.get<size_t>("size");
                std::string md5 = attrs.get<std::string>("md5");
                std::string path = attrs.get<std::string>("path");
                std::string save_as = attrs.get<std::string>("saveAs");
                std::string download = attrs.get<std::string>("download");
                m_required_files.push_back(RequiredFile{type, id, size, md5, path, save_as, download});
            }
        }
    }
}

const std::vector<RequiredFilesResponse::RequiredFile>& RequiredFilesResponse::required_files() const
{
    return m_required_files;
}
