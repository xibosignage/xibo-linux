#pragma once

#include "common/parsing/Parsing.hpp"
#include "common/PlayerRuntimeError.hpp"
#include "control/media/Media.hpp"
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

    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    std::unique_ptr<Xibo::Media> mediaFrom(const XmlNode& node,
                                           int parentWidth,
                                           int parentHeight,
                                           bool globalStatEnabled);

protected:
    virtual MediaOptions::Type typeFrom(const XmlNode& node);
    virtual int idFrom(const XmlNode& node);
    virtual Uri uriFrom(const XmlNode& node);
    virtual int durationFrom(const XmlNode& node);
    virtual MediaGeometry geometryFrom(const XmlNode& node);
    virtual bool statFrom(const XmlNode& node);
    virtual std::unique_ptr<Xibo::Media> createMedia(const MediaOptions& options,
                                                     const XmlNode& node,
                                                     int width,
                                                     int height) = 0;

private:
    MediaOptions baseOptionsFrom(const XmlNode& node);
    void attach(Xibo::Media& media, const XmlNode& node);

    std::unique_ptr<TransitionExecutor> inTransitionFrom(const XmlNode& node,
                                                         const std::shared_ptr<Xibo::Widget>& view);
    std::unique_ptr<TransitionExecutor> outTransitionFrom(const XmlNode& node,
                                                          const std::shared_ptr<Xibo::Widget>& view);

    template <Transition::Heading heading>
    std::unique_ptr<TransitionExecutor> createTransition(Transition::Type typeFrom,
                                                         Transition::Direction direction,
                                                         int durationFrom,
                                                         const std::shared_ptr<Xibo::Widget>& view);

private:
    bool globalStatEnabled_;
};
