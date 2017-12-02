#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Parser.hpp"

#include <memory>
#include <optional>
#include <iostream>

namespace utilities
{
    template <typename Parser> std::shared_ptr<Parser> get_parser(const boost::property_tree::ptree& tree)
    {
        return std::make_shared<Parser>(tree);
    }

}

#endif // UTILITIES_HPP
