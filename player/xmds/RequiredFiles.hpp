#ifndef REQUIREDFILES_HPP
#define REQUIREDFILES_HP

#include "field.hpp"
#include "soap.hpp"

#include <variant>

namespace RequiredFiles
{
    class Response
    {
    public:
        enum class FileType
        {
            Media,
            Layout,
            Resource,
            Invalid
        };

        enum class DownloadType
        {
            HTTP,
            XMDS,
            Invalid
        };

        struct required_file
        {
            int id;
            size_t size;
            std::string md5;
            std::string path;
            std::string filename;
            DownloadType downloadType;
            FileType fileType;
        };

        struct required_resource
        {
            int layoutId;
            int regionId;
            int mediaId;
        };

        const std::vector<required_file>& requiredFiles() const;
        const std::vector<required_resource>& requiredResources() const;

        void addFile(const boost::property_tree::ptree& attrs);

    private:
        std::vector<required_file> m_requiredMedia;
        std::vector<required_resource> m_requiredResources;

    };

    struct Request
    {
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
    };
}

template<>
struct soap::request_traits<RequiredFiles::Request>
{
    static inline const std::string name = "RequiredFiles";
    using response_t = RequiredFiles::Response;
};


#endif // REQUIREDFILES_HPP
