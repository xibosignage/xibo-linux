#include "MediaParser.hpp"
#include "MediaFactory.hpp"
#include "utilities.hpp"

#include <iostream>

MediaParser::MediaParser(const boost::property_tree::ptree& _node) : Parser(_node)
{

}

Media* MediaParser::Parse()
{
    auto media = GetAttributes(m_tree.get_child("<xmlattr>"));

    for(auto&& pair : m_tree)
    {
        if(pair.first == "options")
        {
            std::map<std::string, std::string> options = ParseOptions(pair.second);
            media->InitOptions(options);
        }
    }

    return media;
}

Media* MediaParser::GetAttributes(const boost::property_tree::ptree& tree)
{
    std::cout << "parse media" << std::endl;
    auto media = MediaFactory().createMedia(tree.get<std::string>("type"));
    media->id = tree.get<int>("id");
    media->duration = tree.get<int>("duration");
    media->render = utilities::GetValue<Render>(tree.get<std::string>("render")).value();
    return media;
}
