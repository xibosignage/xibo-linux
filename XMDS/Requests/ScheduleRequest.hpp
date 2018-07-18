#ifndef SCHEDULEREQUEST_HPP
#define SCHEDULEREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class ScheduleRequest : public BaseRequest<SCHEDULE>
{
public:
    using ResponseType = ScheduleResponse;
};

#endif // SCHEDULEREQUEST_HPP
