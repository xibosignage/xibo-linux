#pragma once

#include <boost/core/ignore_unused.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common/Parsing.hpp"
#include "networking/ResponseResult.hpp"

namespace Soap
{
    template <typename Result>
    class BaseResponseParser
    {
    public:
        using OptionalParsedNode = boost::optional<PtreeNode&>;
        using ParsedNode = PtreeNode;

        BaseResponseParser(const std::string& response) : response_(response)
        {
            tryParseResponse(response);
        }
        virtual ~BaseResponseParser() = default;

        ResponseResult<Result> get()
        {
            if (responseTree_)
            {
                auto [name, bodyNode] = responseTree_->front();
                boost::ignore_unused(name);

                if (auto node = getFaultNode())
                {
                    return std::pair{makeErrorFromNode(node), Result{}};
                }

                return parseBodyImpl(bodyNode);
            }

            return std::pair{PlayerError{PlayerError::Type::SOAP, response_}, Result{}};
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
                responseTree_ =
                    Parsing::xmlFromString(response).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
            }
            catch (std::exception&)
            {
            }
        }

        OptionalParsedNode getFaultNode()
        {
            return responseTree_->get_child_optional("SOAP-ENV:Fault");
        }

        PlayerError makeErrorFromNode(OptionalParsedNode faultNode)
        {
            auto faultMessage = faultNode->template get<std::string>("faultstring");
            return PlayerError{PlayerError::Type::SOAP, faultMessage};
        }

    private:
        std::string response_;
        boost::optional<ParsedNode> responseTree_;
    };

}
