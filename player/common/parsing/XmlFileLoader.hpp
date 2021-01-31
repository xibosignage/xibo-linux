#pragma once

#include "Parsing.hpp"

class XmlFileLoader
{
public:
    virtual ~XmlFileLoader() = default;
    virtual XmlNode loadXmlFrom(const FilePath& file) = 0;
};
