#ifndef LAYOUTPARSER_HPP
#define LAYOUTPARSER_HPP

#include "Parser.hpp"
#include "Layout.hpp"

class LayoutParser : public Parser
{
public:
    LayoutParser(wxXmlNode* _node);

    Layout* Parse() override;
    Layout* GetAttributes(wxXmlNode* node) override;
};

#endif // LAYOUTPARSER_HPP
