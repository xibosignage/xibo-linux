#pragma once

#include "MediaOptions.hpp"

class ImageOptions : public MediaOptions
{
public:
    ImageOptions(int id,
                 boost::optional<std::string> uri,
                 int duration,
                 boost::optional<std::string> scaleType,
                 boost::optional<std::string> align,
                 boost::optional<std::string> valign);
    ImageOptions(const xml_node& node);

    boost::optional<std::string> scaleType() const;
    boost::optional<std::string> align() const;
    boost::optional<std::string> valign() const;

private:
    boost::optional<std::string> m_scaleType;
    boost::optional<std::string> m_align;
    boost::optional<std::string> m_valign;
};
