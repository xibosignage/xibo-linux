#pragma once

#include "XmlFileLoader.hpp"

class XmlFileLoaderMissingRoot : public XmlFileLoader
{
public:
    XmlFileLoaderMissingRoot(const NodePath& rootNodeName);
    XmlNode loadXmlFrom(const FilePath& file) override;

private:
    NodePath rootNodeName_;
};
