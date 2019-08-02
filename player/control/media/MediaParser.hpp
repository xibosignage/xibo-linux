#pragma once

#include "constants.hpp"
#include "MediaOptions.hpp"
#include "IMedia.hpp"

#include <boost/property_tree/ptree.hpp>

std::istream& operator >>(std::istream& in, MediaGeometry::ScaleType& scaleType);
std::istream& operator >>(std::istream& in, MediaGeometry::Align& align);
std::istream& operator >>(std::istream& in, MediaGeometry::Valign& valign);

std::istream& operator >>(std::istream& in, Transition::Type& type);
std::istream& operator >>(std::istream& in, Transition::Direction& direction);

class IMedia;
class TransitionExecutor;

class MediaParser
{
public:
    virtual ~MediaParser() = default;
    virtual std::unique_ptr<IMedia> mediaFrom(const ptree_node& node);

protected:
    virtual MediaOptions::Type typeFrom(const ptree_node& node);
    virtual int idFrom(const ptree_node& node);
    virtual Uri uriFrom(const ptree_node& node);
    virtual int durationFrom(const ptree_node& node);
    virtual MediaGeometry geometryFrom(const ptree_node& node);
    virtual ExtraOptions parseExtraOptionsImpl(const ptree_node& node) = 0;
    virtual void attachAdditionalMedia(IMedia& media, const ptree_node& node);

private:
    MediaOptions baseOptionsFrom(const ptree_node& node);
    ExtraOptions parseExtraOptions(const ptree_node& node);
    std::unique_ptr<IMedia> createMedia(const MediaOptions& options, const ExtraOptions& extraOptions);

    std::unique_ptr<TransitionExecutor> parseInTransition(const ptree_node& node, const std::shared_ptr<IWidget>& view);
    std::unique_ptr<TransitionExecutor> parseOutTransition(const ptree_node& node, const std::shared_ptr<IWidget>& view);

    template<Transition::Heading heading>
    std::unique_ptr<TransitionExecutor> createTransition(Transition::Type typeFrom,
                                                         Transition::Direction direction,
                                                         int durationFrom,
                                                         const std::shared_ptr<IWidget>& view);

};
