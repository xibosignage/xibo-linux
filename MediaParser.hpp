#ifndef MEDIAPARSER_HPP
#define MEDIAPARSER_HPP

#include "Parser.hpp"
#include "Media.hpp"
#include "Image.hpp"

#include <spdlog/spdlog.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace ParserHelper
{
    template <typename T>
    T from_string(const std::string& option_name);

    template <>
    inline Image::ScaleType from_string(const std::string& option_name)
    {
        if(option_name == "center")
            return Image::ScaleType::Center;
        else if(option_name == "stretch")
            return Image::ScaleType::Stretch;
        else
            return Image::ScaleType::Invalid;
    }

    template <>
    inline Image::Align from_string(const std::string& option_name)
    {
        if(option_name == "left")
            return Image::Align::Left;
        else if(option_name == "center")
            return Image::Align::Center;
        else if(option_name == "right")
            return Image::Align::Right;
        else
            return Image::Align::Invalid;
    }

    template <>
    inline Image::Valign from_string(const std::string& option_name)
    {
        if(option_name == "top")
            return Image::Valign::Top;
        else if(option_name == "middle")
            return Image::Valign::Middle;
        else if(option_name == "bottom")
            return Image::Valign::Bottom;
        else
            return Image::Valign::Invalid;
    }
}

template <typename T>
class MediaParser : public Parser<T>
{
public:
    using Parser<T>::Parser;

    std::shared_ptr<T> parse() override
    {
        spdlog::get(LOGGER)->debug("parse media");

        auto attrs = this->m_tree.get_child("<xmlattr>");
        auto options = this->m_tree.get_child("options");

        m_id = attrs.template get<int>("id");
        m_duration = attrs.template get<int>("duration");
        m_uri = options.template get<std::string>("uri");

        return this->create_from_attrs(options);
    }

protected:
    int m_id;
    int m_duration;
    std::string m_uri;

};

#endif // MEDIAPARSER_HPP
