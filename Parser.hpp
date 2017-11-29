#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <memory>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class XiboObject;

class Parser
{
public:
    Parser(const boost::property_tree::ptree& tree);
    virtual ~Parser() = default;

    virtual XiboObject* InitObject() = 0;
    virtual XiboObject* Parse() = 0;

protected:
    boost::property_tree::ptree m_tree;

};

#endif // PARSER_HPP
