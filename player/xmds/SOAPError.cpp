#include "SOAPError.hpp"

#include "utils/Logger.hpp"

SOAP::Error::Error(const std::string& faultCode, const std::string& faultMessage) :
    m_faultCode(faultCode), m_faultMessage(faultMessage)
{
}

const std::string& SOAP::Error::faultCode() const
{
    return m_faultCode;
}

const std::string& SOAP::Error::faultMessage() const
{
    return m_faultMessage;
}

SOAP::Error::operator bool() const
{
    return !m_faultCode.empty() || !m_faultMessage.empty();
}
