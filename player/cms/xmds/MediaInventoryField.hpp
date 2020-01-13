#pragma once

#include "cms/xmds/MediaInventoryItem.hpp"
#include "common/SoapField.hpp"

template <>
class SoapField<MediaInventoryItems> : public NamedField<MediaInventoryItems>
{
public:
    using NamedField<MediaInventoryItems>::NamedField;
    using NamedField<MediaInventoryItems>::operator=;

    std::string_view type() const;
    std::string value() const;

private:
    std::string toXmlString(const MediaInventoryItems& items) const;
};
