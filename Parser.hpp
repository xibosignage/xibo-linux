#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

template <typename T>
class Parser
{
public:
    Parser(const boost::property_tree::ptree& tree) : m_tree(tree) { }

    std::shared_ptr<T> create_from_attrs() { }
    std::shared_ptr<T> parse() { }

protected:
    boost::property_tree::ptree m_tree;

};

#endif // PARSER_HPP
