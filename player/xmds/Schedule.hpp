#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include "field.hpp"
#include "soap.hpp"

namespace Schedule
{
    struct response
    {
        std::string schedule_xml;
    };

    struct request
    {
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
    };
}

template<>
struct soap::request_traits<Schedule::request>
{
    static inline const std::string name = "Schedule";
    using response_t = Schedule::response;
};


#endif // SCHEDULE_HPP
