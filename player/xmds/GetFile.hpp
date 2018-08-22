#ifndef GETFILE_HPP
#define GETFILE_HPP

#include "field.hpp"
#include "soap.hpp"

namespace GetFile
{
    struct Response
    {
        std::string base64chunk;
    };

    struct Request
    {
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
        Field<std::string> file_id{"fileId"};
        Field<std::string> file_type{"fileType"};
        Field<double> chunk_offset{"chunkOffset"};
        Field<double> chunk_size{"chuckSize"};
    };
}

template<>
struct soap::request_traits<GetFile::Request>
{
    static inline const std::string name = "GetFile";
    using response_t = GetFile::Response;
};

#endif // GETFILE_HPP
