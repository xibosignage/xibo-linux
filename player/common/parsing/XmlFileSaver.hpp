#pragma once

#include "Parsing.hpp"

class XmlFileSaver
{
public:
    virtual ~XmlFileSaver() = default;
    virtual void saveXmlTo(const FilePath& file, const XmlNode& tree) = 0;
};
