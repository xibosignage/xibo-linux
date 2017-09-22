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
    layout->schemaVersion = utilities::GetValue<int>(node->GetAttribute("schemaVersion")).value();
    layout->width = utilities::GetValue<int>(node->GetAttribute("width")).value();
    layout->height = utilities::GetValue<int>(node->GetAttribute("height")).value();
    layout->backgroundImage = utilities::GetValue<wxString>(node->GetAttribute("background")).value();
    layout->backgroundColor = utilities::GetValue<wxString>(node->GetAttribute("bgcolor")).value();
    return layout;
}
