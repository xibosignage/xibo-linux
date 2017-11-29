#include "MediaParser.hpp"
#include "MediaFactory.hpp"
#include "utilities.hpp"

#include <iostream>

MediaParser::MediaParser(const boost::property_tree::ptree& _node) : Parser(_node)
{

}

Media* MediaParser::Parse()
{
    auto media = InitObject();

    return media;
}

Media* MediaParser::InitObject()
{
    std::cout << "parse media" << std::endl;

    auto attrs = m_tree.get_child("<xmlattr>");

    auto media = MediaFactory().createMedia(attrs.get<std::string>("type"));
    media->id = attrs.get<int>("id");
    media->duration = attrs.get<int>("duration");
    media->render = utilities::GetValue<Render>(attrs.get<std::string>("render")).value();
    media->uri = m_tree.get_child("options").get<std::string>("uri");

    return media;
}
