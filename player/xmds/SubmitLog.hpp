#ifndef SUBMITLOG_HPP
#define SUBMITLOG_HPP

#include "field.hpp"
#include "soap.hpp"

namespace SubmitLog
{
    struct response
    {
        bool success;
    };

    struct request
    {
        field<std::string> server_key{"serverKey"};
        field<std::string> hardware_key{"hardwareKey"};
        field<std::string> log_xml{"logXml"};
    };
}

template<>
struct soap::request_traits<SubmitLog::request>
{
    static inline const std::string name = "SubmitLog";
    using response_t = SubmitLog::response;
};

#endif // SUBMITLOG_HPP
