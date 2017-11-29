#ifndef LAYOUTPARSER_HPP
#define LAYOUTPARSER_HPP

#include "Parser.hpp"
#include "Layout.hpp"

class LayoutParser : public Parser
{
public:
    LayoutParser(const boost::property_tree::ptree& _node);

    Layout* Parse() override;
    Layout* GetAttributes(const boost::property_tree::ptree& node) override;
};

#endif // LAYOUTPARSER_HPP
