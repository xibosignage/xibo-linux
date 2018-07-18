#ifndef GETFILEREQUEST_HPP
#define GETFILEREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class GetFileRequest : public BaseRequest<GET_FILE, std::string, std::string, double, double>
{
public:
    using ResponseType = GetFileResponse;

    GetFileRequest();
    int file_id() const;
    void file_id(int value);
    std::string file_type() const;
    void file_type(const std::string& value);
    double chunk_offset() const;
    void chunk_offset(double value);
    double chunk_size() const;
    void chunk_size(double value);
};

#endif // GETFILEREQUEST_HPP
