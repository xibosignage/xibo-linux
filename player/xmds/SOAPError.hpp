#pragma once

#include <string>

namespace SOAP
{
    class Error
    {
    public:
        Error() = default;
        Error(const std::string& faultCode, const std::string& faultMessage);

        const std::string& faultCode() const;
        const std::string& faultMessage() const;
        operator bool() const;

    private:
        std::string m_faultCode;
        std::string m_faultMessage;

    };
}
