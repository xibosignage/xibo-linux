#pragma once

#include "constants.hpp"
#include "control/media/Media.hpp"
#include "control/media/MediaOptions.hpp"
#include "control/transitions/Transition.hpp"

std::istream& operator>>(std::istream& in, MediaGeometry::ScaleType& scaleType);
std::istream& operator>>(std::istream& in, MediaGeometry::Align& align);
std::istream& operator>>(std::istream& in, MediaGeometry::Valign& valign);

std::istream& operator>>(std::istream& in, Transition::Type& type);
std::istream& operator>>(std::istream& in, Transition::Direction& direction);

class TransitionExecutor;

class MediaParser
{
public:
    virtual ~MediaParser() = default;

    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    std::unique_ptr<Xibo::Media> mediaFrom(const PtreeNode& node, int parentWidth, int parentHeight);

protected:
    virtual MediaOptions::Type typeFrom(const PtreeNode& node);
    virtual int idFrom(const PtreeNode& node);
    virtual Uri uriFrom(const PtreeNode& node);
    virtual int durationFrom(const PtreeNode& node);
    virtual MediaGeometry geometryFrom(const PtreeNode& node);
    virtual std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& options,
                                                     const PtreeNode& node,
                                                     int width,
                                                     int height) = 0;

private:
    MediaOptions baseOptionsFrom(const PtreeNode& node);
    void attach(Xibo::Media& media, const PtreeNode& node);

    std::unique_ptr<TransitionExecutor> inTransitionFrom(const PtreeNode& node,
                                                         const std::shared_ptr<Xibo::Widget>& view);
    std::unique_ptr<TransitionExecutor> outTransitionFrom(const PtreeNode& node,
                                                          const std::shared_ptr<Xibo::Widget>& view);

    template <Transition::Heading heading>
    std::unique_ptr<TransitionExecutor> createTransition(Transition::Type typeFrom,
                                                         Transition::Direction direction,
                                                         int durationFrom,
                                                         const std::shared_ptr<Xibo::Widget>& view);
};
