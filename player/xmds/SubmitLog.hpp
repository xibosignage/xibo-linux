#ifndef SUBMITLOG_HPP
#define SUBMITLOG_HPP

#include "field.hpp"
#include "soap.hpp"

namespace SubmitLog
{
    struct Response
    {
        bool success;
    };

    struct Request
    {
        Field<std::string> server_key{"serverKey"};
        Field<std::string> hardware_key{"hardwareKey"};
        Field<std::string> log_xml{"logXml"};
    };
}

template<>
struct soap::request_traits<SubmitLog::Request>
{
    static inline const std::string name = "SubmitLog";
    using response_t = SubmitLog::Response;
};

#endif // SUBMITLOG_HPP
