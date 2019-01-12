#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/system/error_code.hpp>

#include "SOAPError.hpp"
#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"

namespace SOAP
{
    template<typename Result>
    class BaseResponseParser
    {
    public:
        using OptionalParsedNode = boost::optional<boost::property_tree::ptree&>;
        using ParsedNode = boost::property_tree::ptree;

        BaseResponseParser(const std::string& response) : m_response(response)
        {
            tryParseResponse(response);
        }
        virtual ~BaseResponseParser() = default;

        std::pair<SOAP::Error, Result> get()
        {
            if(m_responseTree)
            {
                auto [name, bodyNode] = m_responseTree->front();

                if(auto node = getFaultNode())
                {
                    return std::pair{makeErrorFromNode(node), Result{}};
                }

                return std::pair{SOAP::Error{}, doParse(bodyNode)};
            }

            return std::pair{SOAP::Error{"Parser", m_response}, Result{}};
        }

    protected:
        virtual Result doParse(const ParsedNode& node) = 0;

    private:
        void tryParseResponse(const std::string& response)
        {
            try
            {
                m_responseTree = Utils::parseXmlFromString(response).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
            }
            catch(std::exception&)
            {
            }
        }

        OptionalParsedNode getFaultNode()
        {
            return m_responseTree->get_child_optional("SOAP-ENV:Fault");
        }

        SOAP::Error makeErrorFromNode(OptionalParsedNode faultNode)
        {
            auto faultCode = faultNode->template get<std::string>("faultcode");
            auto faultMessage = faultNode->template get<std::string>("faultstring");

            return SOAP::Error{faultCode, faultMessage};
        }

    private:
        std::string m_response;
        boost::optional<ParsedNode> m_responseTree;

    };

}
