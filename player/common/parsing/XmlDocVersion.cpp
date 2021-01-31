#include "XmlDocVersion.hpp"

bool operator==(const XmlDocVersion& first, const XmlDocVersion& second)
{
    return static_cast<std::string>(first) == static_cast<std::string>(second);
}

bool operator!=(const XmlDocVersion& first, const XmlDocVersion& second)
{
    return !(first == second);
}

std::istream& operator>>(std::istream& in, XmlDocVersion& version)
{
    return in >> version.value_;
}

std::ostream& operator<<(std::ostream& out, const XmlDocVersion& version)
{
    return out << version.value_;
}
