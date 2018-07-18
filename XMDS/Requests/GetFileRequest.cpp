#include "GetFileRequest.hpp"

GetFileRequest::GetFileRequest() : BaseRequest("fileId", "fileType", "chunkOffset", "chuckSize")
{
}

int GetFileRequest::file_id() const
{
    return std::stoi(field<0>().value());
}

void GetFileRequest::file_id(int value)
{
    field<0>().set_value(std::to_string(value));
}

std::string GetFileRequest::file_type() const
{
    return field<1>().value();
}

void GetFileRequest::file_type(const std::string& value)
{
    field<1>().set_value(value);
}

double GetFileRequest::chunk_offset() const
{
    return field<2>().value();
}

void GetFileRequest::chunk_offset(double value)
{
    field<2>().set_value(value);
}

double GetFileRequest::chunk_size() const
{
    return field<3>().value();
}

void GetFileRequest::chunk_size(double value)
{
    field<3>().set_value(value);
}
