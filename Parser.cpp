#include "Parser.hpp"

Parser::Parser(const boost::property_tree::ptree& tree) : m_tree(tree)
{

}

std::map<std::string, std::string> Parser::ParseOptions(const boost::property_tree::ptree& currentTree)
{
    std::map<std::string, std::string> map;
    for(auto&& pair : currentTree)
    {
        map.insert(std::make_pair(pair.first, pair.second.get<std::string>("")));
    }

    return map;
}
