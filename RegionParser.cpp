#include "RegionParser.hpp"
#include "MediaParser.hpp"
#include "utilities.hpp"

#include <iostream>

RegionParser::RegionParser(wxXmlNode* _node) : Parser(_node)
{

}

Region* RegionParser::Parse()
{
    auto region = GetAttributes(m_node);

    auto currentNode = m_node->GetChildren();
    while(currentNode)
    {
        if(currentNode->GetName() == "media")
        {
            auto media = std::shared_ptr<Media>(utilities::GetParser<MediaParser>(currentNode)->Parse());
            region->medias.push_back(media);
        }
        else if(currentNode->GetName() == "options")
        {
            std::map<wxString, wxString> options = ParseOptions(currentNode->GetChildren());
            region->InitOptions(options);
        }
        currentNode = currentNode->GetNext();
    }

    return region;
}

Region* RegionParser::GetAttributes(wxXmlNode* node)
{
    std::cout << "parse region" << std::endl;
    Region* region = new Region;
    region->id = utilities::GetValue<int>(node->GetAttribute("id")).value();
    region->width = utilities::GetValue<int>(node->GetAttribute("width")).value();
    region->height = utilities::GetValue<int>(node->GetAttribute("height")).value();
    region->top = utilities::GetValue<int>(node->GetAttribute("top")).value();
    region->left = utilities::GetValue<int>(node->GetAttribute("left")).value();
    return region;
}
