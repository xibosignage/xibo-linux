#ifndef SUBMITSTATS_HPP
#define SUBMITSTATS_HPP

#include "field.hpp"
#include "soap.hpp"

namespace SubmitStats
{
    struct Response
    {
        bool success;
    };

    struct Request
    {
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
        Field<std::string> stat_xml{"statXml"};
    };
}

template<>
struct soap::request_traits<SubmitStats::Request>
{
    static inline const std::string name = "SubmitStats";
    using response_t = SubmitStats::Response;
};

#endif // SUBMITSTATS_HPP
