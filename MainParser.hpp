#pragma once

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <vector>

class MainLayout;

class MainParser
{
public:
    MainParser(const std::string& file_name);
    std::unique_ptr<MainLayout> parse();

private:
    void parse_xml_tree();

private:
    std::unique_ptr<MainLayout> m_layout;
    boost::property_tree::ptree m_tree;

};
