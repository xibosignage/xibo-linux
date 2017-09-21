#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <memory>

#include <wx/string.h>
#include <wx/xml/xml.h>

class XiboObject;

class Parser
{
public:
    Parser(wxXmlNode* _node);
    virtual XiboObject* GetAttributes(wxXmlNode* node) = 0;
    virtual XiboObject* Parse() = 0;
    std::map<wxString, wxString> ParseOptions(wxXmlNode* currentNode);

protected:
    wxXmlNode* m_node;

};

#endif // PARSER_HPP
