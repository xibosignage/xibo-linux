#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "Parser.hpp"
#include "MediaParser.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include "constants.hpp"
#include <any>

#include <memory>
#include <optional>
#include <iostream>

namespace utilities
{
    template <typename Parser> std::shared_ptr<Parser> GetParser(const boost::property_tree::ptree& tree)
    {
        return std::make_shared<Parser>(tree);
    }

    template <typename T> std::optional<T> GetValue(const std::string& optionName)
    {
        if(!optionName.empty())
            return std::stoi(optionName);
        return {};
    }

    template <> std::optional<std::string> GetValue(const std::string& optionName);
    template <> std::optional<ScaleType> GetValue(const std::string& optionName);
    template <> std::optional<Align> GetValue(const std::string& optionName);
    template <> std::optional<Valign> GetValue(const std::string& optionName);
    template <> std::optional<Render> GetValue(const std::string& optionName);

}

#endif // UTILITIES_HPP
