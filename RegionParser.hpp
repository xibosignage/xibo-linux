#ifndef REGIONPARSER_HPP
#define REGIONPARSER_HPP

#include "Parser.hpp"
#include "Region.hpp"

class RegionParser : public Parser
{
public:
    RegionParser(wxXmlNode* _node);

    Region* Parse() override;
    Region* GetAttributes(wxXmlNode* node) override;
};

#endif // REGIONPARSER_HPP
