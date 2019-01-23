#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/system/error_code.hpp>

#include "utils/Logger.hpp"
#include "utils/Utilities.hpp"
#include "utils/ResponseResult.hpp"

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

        ResponseResult<Result> get()
        {
            if(m_responseTree)
            {
                auto [name, bodyNode] = m_responseTree->front();

                if(auto node = getFaultNode())
                {
                    return std::pair{makeErrorFromNode(node), Result{}};
                }

                return std::pair{boost::system::error_code{}, doParse(bodyNode)};
            }

            // FIXME custom error
            return std::pair{boost::system::error_code{}, Result{}};
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

        boost::system::error_code makeErrorFromNode(OptionalParsedNode faultNode)
        {
            auto faultCode = faultNode->template get<std::string>("faultcode");
            auto faultMessage = faultNode->template get<std::string>("faultstring");

            return boost::system::error_code{}; // FIXME custom error
        }

    private:
        std::string m_response;
        boost::optional<ParsedNode> m_responseTree;

    };

}
