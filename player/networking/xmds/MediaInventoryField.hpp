#pragma once

#include "common/Field.hpp"
#include "networking/xmds/MediaInventoryItem.hpp"

template <>
class Field<MediaInventoryItems>
{
public:
    explicit Field(std::string_view name);

    Field& operator=(MediaInventoryItems&& items);
    std::string_view type() const;
    std::string_view name() const;
    void setValue(MediaInventoryItems&& items);
    std::string_view value() const;

private:
    std::string toXmlString(MediaInventoryItems&& items);

private:
    std::string fieldName_;
    std::string xmlItems_;
};
