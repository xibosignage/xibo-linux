#ifndef SUBMITSTATSREQUEST_HPP
#define SUBMITSTATSREQUEST_HPP

#include "BaseRequest.hpp"
#include "Responses/ResponsesFwd.hpp"

class SubmitStatsRequest : public BaseRequest<SUBMIT_STATS, std::string>
{
public:
    using ResponseType = SubmitStatsResponse;

    SubmitStatsRequest();
    std::string stat_xml() const;
    void stat_xml(const std::string& value);
};

#endif // SUBMITSTATSREQUEST_HPP
