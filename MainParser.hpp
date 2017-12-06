#ifndef LAYOUTPARSER_HPP
#define LAYOUTPARSER_HPP

#include "Parser.hpp"
#include "MainLayout.hpp"

class MainParser : public Parser<MainLayout>
{
public:
    MainParser(const std::string& file_name);

    std::shared_ptr<MainLayout> parse() override;
    std::shared_ptr<MainLayout> create_from_attrs(const boost::property_tree::ptree& attrs) override;
};

#endif // LAYOUTPARSER_HPP
