#pragma once

#include <boost/core/ignore_unused.hpp>

#include "common/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "networking/ResponseResult.hpp"

template <typename Response>
class SoapResponseParser
{
public:
    struct ParseError : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

public:
    XmlNode parse(const std::string& response)
    {
        auto responseTree = parseResponseTree(response);
        if (!responseTree) throw ParseError{"SOAP", "Error while parsing the envelope"};
        if (auto node = faultNode(responseTree)) throw ParseError{errorFromFaultNode(node)};

        auto [name, bodyNode] = responseTree->front();
        boost::ignore_unused(name);
        return bodyNode;
    }

private:
    boost::optional<XmlNode> parseResponseTree(const std::string& response)
    {
        boost::optional<XmlNode> responseTree;
        try
        {
            responseTree = Parsing::xmlFrom(response).get_child("SOAP-ENV:Envelope").get_child("SOAP-ENV:Body");
        }
        catch (std::exception&)
        {
        }
        return responseTree;
    }

    boost::optional<XmlNode&> faultNode(boost::optional<XmlNode> responseTree)
    {
        return responseTree->get_child_optional("SOAP-ENV:Fault");
    }

    PlayerError errorFromFaultNode(const boost::optional<XmlNode&>& faultNode)
    {
        auto faultMessage = faultNode->template get<std::string>("faultstring");
        return PlayerError{"SOAP", faultMessage};
    }
};

template <typename Response>
class SoapResponse
{
public:
    struct ParseError : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

public:
    static Response fromXmlString(const std::string& xmlString)
    {
        SoapResponseParser<Response> parser;
        Response response;
        response.parseBodyTest(parser.parse(xmlString));
        return response;
    }

protected:
    virtual void parseBody(const XmlNode& node) = 0;
    ~SoapResponse() = default;

private:
    void parseBodyTest(const XmlNode& node)
    {
        using namespace std::string_literals;
        try
        {
            parseBody(node);
        }
        catch (std::exception& e)
        {
            throw ParseError{"XMDS", "Error while parsing response. Details: "s + e.what()};
        }
    }
};
