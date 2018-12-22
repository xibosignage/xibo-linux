#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/system/error_code.hpp>

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"

namespace SOAP
{
    template<typename Response>
    class BaseResponseParser
    {
    public:
        using OptionalParsedNode = boost::optional<boost::property_tree::ptree&>;
        using ParsedNode = boost::property_tree::ptree;

        BaseResponseParser(const std::string& xmlResponse)
        {
            tryParseResponse(xmlResponse);
        }

        std::pair<boost::system::error_code, Response> get()
        {
            if(m_responseTree)
            {
                auto [name, bodyNode] = m_responseTree->front();

                if(auto node = getFaultNode())
                {
                    return std::pair{makeErrorFromNode(node), Response{}};
                }

                return std::pair{boost::system::error_code{}, doParse(bodyNode)};
            }

            return std::pair{boost::system::error_code{}, Response{}};
        }

    protected:
        virtual Response doParse(const ParsedNode& node) = 0;

    private:
        void tryParseResponse(const std::string& xmlResponse)
        {
            try
            {
                m_responseTree = Utils::parseXmlFromString(xmlResponse).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
            }
            catch(std::exception&)
            {
            }
        }

        OptionalParsedNode getFaultNode()
        {
            return m_responseTree->get_child_optional("SOAP-ENV:Fault");
        }

        boost::system::error_code makeErrorFromNode(OptionalParsedNode faultNode)
        {
            auto faultCode = faultNode->template get<std::string>("faultcode");
            auto faultMessage = faultNode->template get<std::string>("faultstring");

            Log::debug("[{}] {}", faultCode, faultMessage);

            return boost::system::error_code{};
        }

    private:
        boost::optional<ParsedNode> m_responseTree;

    };

}
