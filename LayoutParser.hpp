#ifndef LAYOUTPARSER_HPP
#define LAYOUTPARSER_HPP

#include "Parser.hpp"
#include "Layout.hpp"

class LayoutParser : public Parser<Layout>
{
public:
    LayoutParser(const boost::property_tree::ptree& tree) : Parser<Layout>(tree) { }

    std::shared_ptr<Layout> parse();
    std::shared_ptr<Layout> create_from_attrs();
};

#endif // LAYOUTPARSER_HPP
