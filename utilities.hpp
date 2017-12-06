#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Parser.hpp"
#include "ImageParser.hpp"
#include "VideoParser.hpp"

#include <memory>

namespace utilities
{
    template <typename Parser> std::shared_ptr<Parser> get_parser(const boost::property_tree::ptree& tree)
    {
        return std::make_shared<Parser>(tree);
    }
}

#endif // UTILITIES_HPP
