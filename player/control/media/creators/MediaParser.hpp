#pragma once

#include "constants.hpp"
#include "ParsedMedia.hpp"

#include <boost/property_tree/ptree.hpp>

std::istream& operator >>(std::istream& in, MediaGeometry::ScaleType& scaleType);
std::istream& operator >>(std::istream& in, MediaGeometry::Align& align);
std::istream& operator >>(std::istream& in, MediaGeometry::Valign& valign);

class MediaParser
{
public:
    virtual ~MediaParser() = default;
    virtual ParsedMedia parse(const xml_node& node);

protected:
    const xml_node& node() const;

    virtual MediaOptions::Type type();
    virtual int id();
    virtual Uri uri();
    virtual int duration();
    virtual MediaGeometry geometry();
    virtual ExtraOptions parseAdditonalOptions(const xml_node& node) = 0;
    virtual std::unique_ptr<ParsedMedia> parseAttachedMedia(const xml_node& node);

private:
    xml_node m_node;
};
