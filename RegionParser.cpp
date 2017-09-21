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
    region->id = std::stoi(node->GetAttribute("id").ToStdString());
    region->width = std::stoi(node->GetAttribute("width").ToStdString());
    region->height = std::stoi(node->GetAttribute("height").ToStdString());
    region->top = std::stoi(node->GetAttribute("top").ToStdString());
    region->left = std::stoi(node->GetAttribute("left").ToStdString());
    return region;
}
