#ifndef NOTIFYSTATUSREQUEST_HPP
#define NOTIFYSTATUSREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class NotifyStatusRequest : public BaseRequest<NOTIFY_STATUS, std::string>
{
public:
    using ResponseType = NotifyStatusResponse;

    NotifyStatusRequest();
    std::string status() const;
    void status(const std::string& value);
};

#endif // NOTIFYSTATUSREQUEST_HPP
