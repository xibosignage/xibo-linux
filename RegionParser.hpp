#ifndef REGIONPARSER_HPP
#define REGIONPARSER_HPP

#include "Parser.hpp"
#include "Region.hpp"

class RegionParser : public Parser<Region>
{
public:
    RegionParser(const boost::property_tree::ptree& tree);

    std::shared_ptr<Region> parse() override;
    std::shared_ptr<Region> create_from_attrs(const boost::property_tree::ptree& attrs) override;

};

#endif // REGIONPARSER_HPP
