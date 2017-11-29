#ifndef MEDIAPARSER_HPP
#define MEDIAPARSER_HPP

#include "Parser.hpp"
#include "Media.hpp"

class MediaParser : public Parser
{
public:
    MediaParser(const boost::property_tree::ptree& _node);

    Media* Parse() override;
    Media* InitObject() override;
};

#endif // MEDIAPARSER_HPP
