#pragma once

#include "common/types/internal/StrongType.hpp"
#include <string>

class XmlDocVersion : public StrongType<std::string>
{
public:
    using StrongType::StrongType;

    friend std::istream& operator>>(std::istream& in, XmlDocVersion& version);
    friend std::ostream& operator<<(std::ostream& out, const XmlDocVersion& version);
};

bool operator==(const XmlDocVersion& first, const XmlDocVersion& second);
bool operator!=(const XmlDocVersion& first, const XmlDocVersion& second);

std::istream& operator>>(std::istream& in, XmlDocVersion& version);
std::ostream& operator<<(std::ostream& out, const XmlDocVersion& version);
