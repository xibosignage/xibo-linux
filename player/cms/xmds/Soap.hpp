#pragma once

#include <string>

namespace Soap
{
    template <typename Response>
    Response createResult(const std::string& soapResponse);

    template <typename Response>
    class ResponseParser;

    template <typename Response>
    class RequestSerializer;
}
