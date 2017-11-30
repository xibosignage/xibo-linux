#ifndef MEDIAPARSER_HPP
#define MEDIAPARSER_HPP

#include "Parser.hpp"
#include "Media.hpp"
#include "Image.hpp"
//#include "utilities.hpp"

#include <iostream>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

template <typename T>
class MediaParser : public Parser<T>
{
public:
    MediaParser(const boost::property_tree::ptree& _node) : Parser<T>(_node) { }

    std::shared_ptr<Media> Parse()
    {
        std::cout << "parse media" << std::endl;

        auto attrs = this->m_tree.get_child("<xmlattr>");
        auto options = this->m_tree.get_child("options");

        m_id = attrs.template get<int>("id");
        m_duration = attrs.template get<int>("duration");
        //m_render = utilities::GetValue<Render>(attrs.template get<std::string>("render")).value();
        m_uri = options.template get<std::string>("uri");

        return Create(options);
    }

    std::shared_ptr<Media> Create(const boost::property_tree::ptree&);

    int m_id;
    int m_duration;
    Render m_render;
    std::string m_uri;

};

#endif // MEDIAPARSER_HPP
