#pragma once

#include <boost/property_tree/ptree.hpp>

#include "common/Parsing.hpp"
#include "networking/ResponseResult.hpp"

namespace Soap
{
template <typename Result>
class BaseResponseParser
{
public:
    using OptionalParsedNode = boost::optional<ptree_node&>;
    using ParsedNode = ptree_node;

    BaseResponseParser(const std::string& response) : m_response(response)
    {
        tryParseResponse(response);
    }
    virtual ~BaseResponseParser() = default;

    ResponseResult<Result> get()
    {
        if (m_responseTree)
        {
            auto [name, bodyNode] = m_responseTree->front();

            if (auto node = getFaultNode())
            {
                return std::pair{makeErrorFromNode(node), Result{}};
            }

            return parseBodyImpl(bodyNode);
        }

        return std::pair{PlayerError{PlayerError::Type::SOAP, m_response}, Result{}};
    }

protected:
    virtual Result parseBody(const ParsedNode& node) = 0;

private:
    ResponseResult<Result> parseBodyImpl(const ParsedNode& node)
    {
        using namespace std::string_literals;

        try
        {
            return std::pair{PlayerError{}, parseBody(node)};
        }
        catch (std::exception& e)
        {
            std::string error = "Error while parsing response. Details: "s + e.what();

            return std::pair{PlayerError{PlayerError::Type::SOAP, error}, Result{}};
        }
    }

    void tryParseResponse(const std::string& response)
    {
        try
        {
            m_responseTree = Parsing::xmlFromString(response).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
        }
        catch (std::exception&)
        {
        }
    }

    OptionalParsedNode getFaultNode()
    {
        return m_responseTree->get_child_optional("SOAP-ENV:Fault");
    }

    PlayerError makeErrorFromNode(OptionalParsedNode faultNode)
    {
        auto faultMessage = faultNode->template get<std::string>("faultstring");
        return PlayerError{PlayerError::Type::SOAP, faultMessage};
    }

private:
    std::string m_response;
    boost::optional<ParsedNode> m_responseTree;
};

}
