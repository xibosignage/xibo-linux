#ifndef REGIONPARSER_HPP
#define REGIONPARSER_HPP

#include "Parser.hpp"
#include "Region.hpp"

class RegionParser : public Parser<Region>
{
public:
    RegionParser(const boost::property_tree::ptree& tree);

    std::shared_ptr<Region> Parse();
    std::shared_ptr<Region> CreateObjectFromAttrs();

};

#endif // REGIONPARSER_HPP
