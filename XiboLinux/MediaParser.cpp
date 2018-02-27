#include "MediaParser.hpp"

#include "constants.hpp"
#include "XiboApp.hpp"

MediaParser::MediaParser(const boost::property_tree::ptree& tree) : m_tree(tree)
{
    spdlog::get(LOGGER)->debug("parse media");

    auto attrs = m_tree.get_child("<xmlattr>");
    m_options = m_tree.get_child("options");

    m_id = attrs.template get<int>("id");
    m_duration = attrs.template get<int>("duration");
    m_use_duration = attrs.template get<bool>("useDuration");
    m_uri = XiboApp::example_dir() + "/" + get_path(attrs.template get<std::string>("type"));
    spdlog::get(LOGGER)->debug(m_uri);
}

// FIXME
std::string MediaParser::get_path(const std::string& type)
{
    auto uri = m_options.get_optional<std::string>("uri");
    if(!uri || type == "ticker" || type == "forecastio")
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(XiboApp::example_dir() + "/requiredFiles.xml", tree);

        auto required_files = tree.get_child("RequiredFiles").get_child("RequiredFileList");
        for(auto&& required_file : required_files)
        {
            auto file_info = required_file.second;
            if(file_info.get<std::string>("FileType") == "resource" && file_info.get<int>("MediaId") == m_id)
            {
                return file_info.get<std::string>("Path");
            }
        }
        return std::string{};
    }
    return uri.value();
}

Image::ScaleType MediaParser::from_scale_type(const std::string& option_name)
{
    if(option_name == "center")
        return Image::ScaleType::Center;
    else if(option_name == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align MediaParser::from_align(const std::string& option_name)
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

Image::Valign MediaParser::from_valign(const std::string& option_name)
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
