#ifndef REQUIREDFILESRESPONSE_HPP
#define REQUIREDFILESRESPONSE_HPP

#include "XmlResponse.hpp"
#include "constants.hpp"

class RequiredFilesResponse : public XmlResponse<REQUIRED_FILES>
{
public:
    class RequiredFile
    {
    public:
        RequiredFile(const std::string& type, int id, size_t size, const std::string& md5,
                     const std::string& path, const std::string& filename, const std::string& download);
        const std::string& type() const;
        int id() const;
        size_t size() const;
        const std::string& md5() const;
        const std::string& path() const;
        const std::string& filename() const;
        const std::string& download() const;

    private:
        std::string m_type;
        int m_id;
        size_t m_size;
        std::string m_md5;
        std::string m_path;
        std::string m_filename;
        std::string m_download;
    };

    RequiredFilesResponse(const std::string& xml_response);
    const std::vector<RequiredFile>& required_files() const;

private:
    std::vector<RequiredFile> m_required_files;
};

#endif // REQUIREDFILESRESPONSE_HPP
