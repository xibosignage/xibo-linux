#include "MediaInventoryField.hpp"

#include "common/parsing/Parsing.hpp"

std::string_view SoapField<MediaInventoryItems>::type() const
{
    return "string";
}

std::string SoapField<MediaInventoryItems>::value() const
{
    return toXmlString(NamedField::value());
}

std::string SoapField<MediaInventoryItems>::toXmlString(const MediaInventoryItems& items) const
{
    XmlNode root;
    auto&& filesNode = root.put_child("files", {});

    for (auto&& item : items)
    {
        auto&& fileNode = filesNode.add_child("file", {});
        auto&& fileAttrs = fileNode.put_child("<xmlattr>", {});

        fileAttrs.put("type", item.type());
        fileAttrs.put("id", item.id());
        fileAttrs.put("complete", static_cast<int>(item.downloadComplete()));
        fileAttrs.put("md5", item.md5());
        fileAttrs.put("lastChecked", item.lastChecked());
    }

    return std::string("<![CDATA[") + Parsing::xmlTreeToString(root) + "]]>";
}
