#ifndef GETFILE_HPP
#define GETFILE_HPP

#include "field.hpp"
#include "soap.hpp"

namespace GetFile
{
    struct response
    {
        std::string base64chunk;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<std::string> file_id{"fileId"};
        field<std::string> file_type{"fileType"};
        field<double> chunk_offset{"chunkOffset"};
        field<double> chunk_size{"chuckSize"};
    };
}

template<>
struct soap::request_traits<GetFile::request>
{
    static inline const std::string name = "GetFile";
    using response_t = GetFile::response;
};

#endif // GETFILE_HPP
