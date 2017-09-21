#include "LayoutParser.hpp"
#include "RegionParser.hpp"
#include "utilities.hpp"

#include <iostream>

LayoutParser::LayoutParser(wxXmlNode* _node) : Parser(_node)
{

}

Layout* LayoutParser::Parse()
{
    auto layout = GetAttributes(m_node);

    auto currentNode = m_node->GetChildren();
    while(currentNode)
    {
        if(currentNode->GetName() == "region")
        {
            auto region = std::shared_ptr<Region>(utilities::GetParser<RegionParser>(currentNode)->Parse());
            layout->regions.push_back(region);
        }
        currentNode = currentNode->GetNext();
    }

    return layout;
}

Layout* LayoutParser::GetAttributes(wxXmlNode* node)
{
    std::cout << "parse layout" << std::endl;
    Layout* layout = new Layout;
    layout->schemaVersion = std::stoi(node->GetAttribute("schemaVersion").ToStdString());
    layout->width = std::stoi(node->GetAttribute("width").ToStdString());
    layout->height = std::stoi(node->GetAttribute("height").ToStdString());
    layout->backgroundImage = node->GetAttribute("background");
    layout->backgroundColor = node->GetAttribute("bgcolor");
    return layout;
}
