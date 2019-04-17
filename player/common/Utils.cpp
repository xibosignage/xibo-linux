#include "Utils.hpp"

#include "FilePath.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <openssl/md5.h>
#include <boost/beast/core/detail/base64.hpp>

xml_node Utils::parseXmlFromPath(const FilePath& xlfPath)
{
    xml_node tree;
    boost::property_tree::read_xml(xlfPath.string(), tree);
    return tree;
}

xml_node Utils::parseXmlFromString(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    xml_node tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}

xml_node Utils::parseJsonFromString(const std::string& json)
{
    std::stringstream stream;
    stream << json;
    xml_node tree;
    boost::property_tree::read_json(stream, tree);
    return tree;
}

std::string Utils::xmlTreeToEscapedString(const xml_node& node)
{
    std::stringstream sstream;
    boost::property_tree::write_xml(sstream, node);
    std::string xmlStr = sstream.str();

    boost::replace_all(xmlStr, "<", "&lt;");
    boost::replace_all(xmlStr, ">", "&gt;");

    return xmlStr;
}

std::string Utils::md5hash(std::string_view data)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.data()), data.size(), result);

    std::stringstream stream;
    for(unsigned char byte : result)
    {
        stream << boost::format("%02x") % static_cast<short>(byte);
    }
    return stream.str();
}

std::string Utils::toBase64(const std::string& text)
{
    return boost::beast::detail::base64_encode(text);
}

std::string Utils::fromBase64(const std::string& text)
{
    return boost::beast::detail::base64_decode(text);
}
