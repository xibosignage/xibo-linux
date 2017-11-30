#ifndef LAYOUTPARSER_HPP
#define LAYOUTPARSER_HPP

#include "Parser.hpp"
#include "Layout.hpp"

class LayoutParser : public Parser<Layout>
{
public:
    LayoutParser(const boost::property_tree::ptree& _node);

    std::shared_ptr<Layout> Parse();
    std::shared_ptr<Layout> CreateObjectFromAttrs();
};

#endif // LAYOUTPARSER_HPP
