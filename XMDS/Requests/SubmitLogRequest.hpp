#ifndef SUBMITLOGREQUEST_HPP
#define SUBMITLOGREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class SubmitLogRequest : public BaseRequest<SUBMIT_LOG, std::string>
{
public:
    using ResponseType = SubmitLogResponse;

    SubmitLogRequest();
    std::string log_xml() const;
    void log_xml(const std::string& value);
};

#endif // SUBMITLOGREQUEST_HPP
