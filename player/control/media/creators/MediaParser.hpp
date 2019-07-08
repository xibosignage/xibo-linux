#pragma once

#include "constants.hpp"
#include "ParsedMedia.hpp"

#include <boost/property_tree/ptree.hpp>

std::istream& operator >>(std::istream& in, MediaGeometry::ScaleType& scaleType);
std::istream& operator >>(std::istream& in, MediaGeometry::Align& align);
std::istream& operator >>(std::istream& in, MediaGeometry::Valign& valign);

std::istream& operator >>(std::istream& in, Transition::Type& type);
std::istream& operator >>(std::istream& in, Transition::Direction& direction);

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
    boost::optional<Transition> inTransition();
    boost::optional<Transition> outTransition();

private:
    xml_node m_node;
};
