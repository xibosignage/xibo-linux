#ifndef REQUIREDFILES_HPP
#define REQUIREDFILES_HP

#include "field.hpp"
#include "soap.hpp"

namespace RequiredFiles
{
    class response
    {
    public:
        struct required_file
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

        const std::vector<required_file>& required_files() const;
        void add_file(const boost::property_tree::ptree& attrs);

    private:
        std::vector<required_file> m_required_files;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
    };
}

template<>
struct soap::request_traits<RequiredFiles::request>
{
    static inline const std::string name = "RequiredFiles";
    using response_t = RequiredFiles::response;
};


#endif // REQUIREDFILES_HPP
