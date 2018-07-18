#ifndef GETFILERESPONSE_HPP
#define GETFILERESPONSE_HPP

#include "XmlResponse.hpp"
#include "constants.hpp"

class GetFileResponse : public BaseResponse<GET_FILE>
{
public:
    GetFileResponse(const std::string& xml_response);

};

#endif // GETFILERESPONSE_HPP
