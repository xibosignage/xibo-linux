#ifndef REQUIREDFILESRESPONSE_HPP
#define REQUIREDFILESRESPONSE_HPP

#include "XmlResponse.hpp"
#include "constants.hpp"

class RequiredFilesResponse : public XmlResponse<REQUIRED_FILES>
{
public:
    struct RequiredFile
    {
        enum class FileType
        {
            Media,
            Layout,
            Resourse,
            Invalid
        };

        enum class DownloadType
        {
            HTTP,
            XMDS,
            Invalid
        };

        FileType file_type;
        int id;
        size_t size;
        std::string md5;
        std::string path;
        std::string filename;
        DownloadType download_type;
    };

    RequiredFilesResponse(const std::string& xml_response);
    const std::vector<RequiredFile>& required_files() const;

private:
    void add_file(const boost::property_tree::ptree& attrs);

private:
    std::vector<RequiredFile> m_required_files;
};

#endif // REQUIREDFILESRESPONSE_HPP
