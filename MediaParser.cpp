#include "MediaParser.hpp"
#include "MediaFactory.hpp"

#include <iostream>

MediaParser::MediaParser(wxXmlNode* _node) : Parser(_node)
{

}

Media* MediaParser::Parse()
{
    auto media = GetAttributes(m_node);

    auto currentNode = m_node->GetChildren();
    while(currentNode)
    {
        if(currentNode->GetName() == "options")
        {
            std::map<wxString, wxString> options = ParseOptions(currentNode->GetChildren());
            media->InitOptions(options);
        }
        currentNode = currentNode->GetNext();
    }

    return media;
}

Media* MediaParser::GetAttributes(wxXmlNode* node)
{
    std::cout << "parse media" << std::endl;
    auto media = MediaFactory().createMedia(node->GetAttribute("type"));
    media->id = std::stoi(node->GetAttribute("id").ToStdString());
    media->duration = std::stoi(node->GetAttribute("duration").ToStdString());
    //media->render = std::stoi(node->GetAttribute("render").ToStdString())
    return media;
}
