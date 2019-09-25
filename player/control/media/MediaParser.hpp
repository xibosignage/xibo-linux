#pragma once

#include "IMedia.hpp"
#include "MediaOptions.hpp"
#include "constants.hpp"

#include <boost/property_tree/ptree.hpp>

std::istream& operator>>(std::istream& in, MediaGeometry::ScaleType& scaleType);
std::istream& operator>>(std::istream& in, MediaGeometry::Align& align);
std::istream& operator>>(std::istream& in, MediaGeometry::Valign& valign);

std::istream& operator>>(std::istream& in, Transition::Type& type);
std::istream& operator>>(std::istream& in, Transition::Direction& direction);

class IMedia;
class TransitionExecutor;

class MediaParser
{
public:
    virtual ~MediaParser() = default;
    std::unique_ptr<IMedia> mediaFrom(const ptree_node& node, int parentWidth, int parentHeight);

protected:
    virtual MediaOptions::Type typeFrom(const ptree_node& node);
    virtual int idFrom(const ptree_node& node);
    virtual Uri uriFrom(const ptree_node& node);
    virtual int durationFrom(const ptree_node& node);
    virtual MediaGeometry geometryFrom(const ptree_node& node);
    virtual ExtraOptions extraOptionsImpl(const ptree_node& node) = 0;

private:
    MediaOptions baseOptionsFrom(const ptree_node& node);
    ExtraOptions extraOptions(const ptree_node& node, int parentWidth, int parentHeight);
    std::unique_ptr<IMedia> createMedia(const MediaOptions& options, const ExtraOptions& extraOptions);
    void attach(IMedia& media, const ptree_node& node);

    std::unique_ptr<TransitionExecutor> inTransitionFrom(const ptree_node& node, const std::shared_ptr<IWidget>& view);
    std::unique_ptr<TransitionExecutor> outTransitionFrom(const ptree_node& node, const std::shared_ptr<IWidget>& view);

    template <Transition::Heading heading>
    std::unique_ptr<TransitionExecutor> createTransition(Transition::Type typeFrom, Transition::Direction direction,
                                                         int durationFrom, const std::shared_ptr<IWidget>& view);
};
