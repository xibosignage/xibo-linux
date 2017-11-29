#ifndef REGIONPARSER_HPP
#define REGIONPARSER_HPP

#include "Parser.hpp"
#include "Region.hpp"

class RegionParser : public Parser
{
public:
    RegionParser(const boost::property_tree::ptree& tree);

    Region* Parse() override;
    Region* InitObject() override;

};

#endif // REGIONPARSER_HPP
