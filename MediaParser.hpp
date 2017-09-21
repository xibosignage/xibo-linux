#ifndef MEDIAPARSER_HPP
#define MEDIAPARSER_HPP

#include "Parser.hpp"
#include "Media.hpp"

class MediaParser : public Parser
{
public:
    MediaParser(wxXmlNode* _node);

    Media* Parse() override;
    Media* GetAttributes(wxXmlNode* node) override;
};

#endif // MEDIAPARSER_HPP
