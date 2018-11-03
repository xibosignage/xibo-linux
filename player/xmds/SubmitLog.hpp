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
        Field<std::string> serverKey{"serverKey"};
        Field<std::string> hardwareKey{"hardwareKey"};
        Field<std::string> logXml{"logXml"};
    };
}

template<>
struct soap::request_traits<SubmitLog::Request>
{
    static inline const std::string name = "SubmitLog";
    using response_t = SubmitLog::Response;
};

#endif // SUBMITLOG_HPP
