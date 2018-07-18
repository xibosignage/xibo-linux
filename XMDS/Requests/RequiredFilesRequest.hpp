#ifndef REQUIREDFILESREQUEST_HPP
#define REQUIREDFILESREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class RequiredFilesRequest : public BaseRequest<REQUIRED_FILES>
{
public:
    using ResponseType = RequiredFilesResponse;
};

#endif // REQUIREDFILESREQUEST_HPP
