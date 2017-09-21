#include "Parser.hpp"

Parser::Parser(wxXmlNode* _node) : m_node(_node)
{

}

std::map<wxString, wxString> Parser::ParseOptions(wxXmlNode* currentNode)
{
    std::map<wxString, wxString> map;
    while(currentNode)
    {
        if(currentNode->GetChildren())
        {
            map.insert(std::make_pair(currentNode->GetName(), currentNode->GetChildren()->GetContent()));
        }
        currentNode = currentNode->GetNext();
    }
    return map;
}
