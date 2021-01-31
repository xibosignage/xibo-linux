#pragma once

#include <boost/core/ignore_unused.hpp>

#include "common/parsing/Parsing.hpp"
#include "networking/ResponseResult.hpp"

namespace Soap
{
    template <typename Result>
    class BaseResponseParser
    {
    public:
        using OptionalXmlNode = boost::optional<XmlNode&>;

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

            return std::pair{PlayerError{"SOAP", response_}, Result{}};
        }

    protected:
        virtual Result parseBody(const XmlNode& node) = 0;

    private:
        ResponseResult<Result> parseBodyImpl(const XmlNode& node)
        {
            using namespace std::string_literals;

            try
            {
                return std::pair{PlayerError{}, parseBody(node)};
            }
            catch (std::exception& e)
            {
                std::string error = "Error while parsing response. Details: "s + e.what();

                return std::pair{PlayerError{"SOAP", error}, Result{}};
            }
        }

        void tryParseResponse(const std::string& response)
        {
            try
            {
                responseTree_ = Parsing::xmlFrom(response).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
            }
            catch (std::exception&)
            {
            }
        }

        OptionalXmlNode getFaultNode()
        {
            return responseTree_->get_child_optional("SOAP-ENV:Fault");
        }

        PlayerError makeErrorFromNode(OptionalXmlNode faultNode)
        {
            auto faultMessage = faultNode->template get<std::string>("faultstring");
            return PlayerError{"SOAP", faultMessage};
        }

    private:
        std::string response_;
        boost::optional<XmlNode> responseTree_;
    };

}
