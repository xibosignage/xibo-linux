#include "XmlFileLoaderMissingRoot.hpp"

XmlFileLoaderMissingRoot::XmlFileLoaderMissingRoot(const NodePath& rootNodeName) : rootNodeName_(rootNodeName) {}

XmlNode XmlFileLoaderMissingRoot::loadXmlFrom(const FilePath& file)
{
    auto tree = Parsing::xmlFrom(file);
    auto root = tree.get_child_optional(rootNodeName_);
    if (!root.has_value())
    {
        XmlNode newTree;
        newTree.put_child(rootNodeName_, tree);
        return newTree;
    }
    return tree;
}
