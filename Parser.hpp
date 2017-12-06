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
    virtual ~Parser() = default;

    virtual std::shared_ptr<T> parse() = 0;
    virtual std::shared_ptr<T> create_from_attrs(const boost::property_tree::ptree& options) = 0;

protected:
    boost::property_tree::ptree m_tree;

};

#endif // PARSER_HPP
