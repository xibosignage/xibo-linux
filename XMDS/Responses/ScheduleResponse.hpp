#ifndef SCHEDULERESPONSE_HPP
#define SCHEDULERESPONSE_HPP

#include "XmlResponse.hpp"
#include "constants.hpp"

class ScheduleResponse : public XmlResponse<SCHEDULE>
{
public:
    ScheduleResponse(const std::string& xml_response);

};
#endif // SCHEDULERESPONSE_HPP
