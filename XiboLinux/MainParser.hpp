#pragma once

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>
#include <vector>

#include "MainLayout.hpp"
#include "Region.hpp"

class MainParser
{
public:
    MainParser(const std::string& full_path);
    std::unique_ptr<MainLayout> parse();

private:
    void parse_xml_tree();

    template <typename MediaType, typename T, T... N, typename... Args>
    void add_media(std::integer_sequence<T, N...>, const std::tuple<Args...>& args);

    template <typename T, T... N, typename... Args>
    void add_region(std::integer_sequence<T, N...>, const std::tuple<Args...>& args);

private:
    std::unique_ptr<MainLayout> m_layout;
    boost::property_tree::ptree m_tree;

};

template <typename MediaType, typename T, T... N, typename... Args>
void MainParser::add_media(std::integer_sequence<T, N...>, const std::tuple<Args...>& args)
{
    auto&& current_region = m_layout->region(m_layout->regions_count() - 1);
    current_region.add_media<MediaType>(std::get<N>(args)...);
}

template <typename T, T... N, typename... Args>
void MainParser::add_region(std::integer_sequence<T, N...>, const std::tuple<Args...>& args)
{
    m_layout->add_region(std::get<N>(args)...);
}
