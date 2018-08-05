#ifndef SUBMITSTATS_HPP
#define SUBMITSTATS_HPP

#include "field.hpp"
#include "soap.hpp"

namespace SubmitStats
{
    struct response
    {
        bool success;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<std::string> stat_xml{"statXml"};
    };
}

template<>
struct soap::request_traits<SubmitStats::request>
{
    static inline const std::string name = "SubmitStats";
    using response_t = SubmitStats::response;
};

#endif // SUBMITSTATS_HPP
