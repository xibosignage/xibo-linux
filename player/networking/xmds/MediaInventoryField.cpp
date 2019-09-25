#include "MediaInventoryField.hpp"

#include "common/Parsing.hpp"
#include <boost/property_tree/ptree.hpp>

Field<MediaInventoryItems>::Field(std::string_view name) : m_fieldName(name) {}

Field<MediaInventoryItems>& Field<MediaInventoryItems>::operator=(MediaInventoryItems&& items)
{
    setValue(std::move(items));
    return *this;
}

std::string_view Field<MediaInventoryItems>::type() const
{
    return "string";
}

std::string_view Field<MediaInventoryItems>::name() const
{
    return m_fieldName;
}

void Field<MediaInventoryItems>::setValue(MediaInventoryItems&& items)
{
    m_xmlItems = toXmlString(std::move(items));
}

std::string_view Field<MediaInventoryItems>::value() const
{
    return m_xmlItems;
}

std::string Field<MediaInventoryItems>::toXmlString(MediaInventoryItems&& items)
{
    boost::property_tree::ptree root;
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
