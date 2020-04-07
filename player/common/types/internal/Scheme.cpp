#include "common/types/Uri.hpp"

Uri::Scheme::Scheme(const std::string& scheme) : StrongType{scheme}
{
    std::transform(value_.begin(), value_.end(), value_.begin(), [](unsigned char c) { return std::tolower(c); });
}

bool operator==(const Uri::Scheme& first, const Uri::Scheme& second)
{
    return static_cast<std::string>(first) == static_cast<std::string>(second);
}

bool operator!=(const Uri::Scheme& first, const Uri::Scheme& second)
{
    return !(first == second);
}

bool operator<(const Uri::Scheme& first, const Uri::Scheme& second)
{
    return std::less<>{}(static_cast<std::string>(first), static_cast<std::string>(second));
}
