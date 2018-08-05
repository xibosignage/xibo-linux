#ifndef UTILS_HPP
#define UTILS_HPP

#include <boost/property_tree/ptree.hpp>

namespace utils
{
    boost::property_tree::ptree parse_xml(const std::string& xml);
}

#endif // UTILS_HPP
