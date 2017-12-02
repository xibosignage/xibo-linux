#ifndef LAYOUTPARSER_HPP
#define LAYOUTPARSER_HPP

#include "Parser.hpp"
#include "Layout.hpp"

class MainParser : public Parser<Layout>
{
public:
    MainParser(const std::string& file_name);

    std::shared_ptr<Layout> parse();
    std::shared_ptr<Layout> create_from_attrs();
};

#endif // LAYOUTPARSER_HPP
