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
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<std::string> fileId{"fileId"};
        Field<std::string> fileType{"fileType"};
        Field<double> chunkOffset{"chunkOffset"};
        Field<double> chunkSize{"chuckSize"};
    };
}

template<>
struct soap::request_traits<GetFile::Request>
{
    static inline const std::string name = "GetFile";
    using response_t = GetFile::Response;
};

#endif // GETFILE_HPP
