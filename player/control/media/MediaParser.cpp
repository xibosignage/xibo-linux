#include "MediaParser.hpp"

#include "control/media/Media.hpp"
#include "control/media/MediaParsersRepo.hpp"
#include "control/media/MediaResources.hpp"

#include "control/transitions/FadeTransitionExecutor.hpp"
#include "control/transitions/FlyTransitionExecutor.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/fs/Resource.hpp"

namespace MediaResources = XlfResources::Media;

const bool DefaultEnableStat = true;

std::istream& operator>>(std::istream& in, MediaGeometry::ScaleType& scaleType)
{
    std::string temp;
    in >> temp;

    if (temp == MediaResources::Geometry::Scaled || temp == MediaResources::Geometry::Aspect)
        scaleType = MediaGeometry::ScaleType::Scaled;
    else if (temp == MediaResources::Geometry::Stretch)
        scaleType = MediaGeometry::ScaleType::Stretch;

    return in;
}

std::istream& operator>>(std::istream& in, MediaGeometry::Align& align)
{
    std::string temp;
    in >> temp;

    if (temp == MediaResources::Geometry::LeftAlign)
        align = MediaGeometry::Align::Left;
    else if (temp == MediaResources::Geometry::CenterAlign)
        align = MediaGeometry::Align::Center;
    else if (temp == MediaResources::Geometry::RightAlign)
        align = MediaGeometry::Align::Right;

    return in;
}

std::istream& operator>>(std::istream& in, MediaGeometry::Valign& valign)
{
    std::string temp;
    in >> temp;

    if (temp == MediaResources::Geometry::TopValign)
        valign = MediaGeometry::Valign::Top;
    else if (temp == MediaResources::Geometry::MiddleValign)
        valign = MediaGeometry::Valign::Middle;
    else if (temp == MediaResources::Geometry::BottomValign)
        valign = MediaGeometry::Valign::Bottom;

    return in;
}

std::istream& operator>>(std::istream& in, Transition::Type& type)
{
    std::string temp;
    in >> temp;

    if (temp == MediaResources::Tranisiton::Fly)
        type = Transition::Type::Fly;
    else if (temp == MediaResources::Tranisiton::FadeIn || temp == MediaResources::Tranisiton::FadeOut)
        type = Transition::Type::Fade;

    return in;
}

std::istream& operator>>(std::istream& in, Transition::Direction& direction)
{
    std::string temp;
    in >> temp;

    if (temp == MediaResources::Tranisiton::N)
        direction = Transition::Direction::N;
    else if (temp == MediaResources::Tranisiton::NE)
        direction = Transition::Direction::NE;
    else if (temp == MediaResources::Tranisiton::E)
        direction = Transition::Direction::E;
    else if (temp == MediaResources::Tranisiton::SE)
        direction = Transition::Direction::SE;
    else if (temp == MediaResources::Tranisiton::S)
        direction = Transition::Direction::S;
    else if (temp == MediaResources::Tranisiton::SW)
        direction = Transition::Direction::SW;
    else if (temp == MediaResources::Tranisiton::W)
        direction = Transition::Direction::W;
    else if (temp == MediaResources::Tranisiton::NW)
        direction = Transition::Direction::NW;

    return in;
}

std::unique_ptr<Xibo::Media> MediaParser::mediaFrom(const XmlNode& node,
                                                    int parentWidth,
                                                    int parentHeight,
                                                    bool globalStatEnabled)
{
    using namespace std::string_literals;

    try
    {
        globalStatEnabled_ = globalStatEnabled;

        auto baseOptions = baseOptionsFrom(node);
        auto media = createMedia(baseOptions, node, parentWidth, parentHeight);

        media->inTransition(inTransitionFrom(node, media->view()));
        media->outTransition(outTransitionFrom(node, media->view()));

        attach(*media, node);

        return media;
    }
    catch (PlayerRuntimeError& e)
    {
        throw MediaParser::Error{"MediaParser - " + e.domain(), e.message()};
    }
    catch (std::exception& e)
    {
        throw MediaParser::Error{"MediaParser", e.what()};
    }
}

MediaOptions MediaParser::baseOptionsFrom(const XmlNode& node)
{
    auto type = typeFrom(node);
    auto id = idFrom(node);
    auto uri = uriFrom(node);
    auto duration = durationFrom(node);
    auto stat = globalStatEnabled_ && statFrom(node);
    auto geometry = geometryFrom(node);

    return MediaOptions{type, id, uri, duration, stat, geometry};
}

MediaOptions::Type MediaParser::typeFrom(const XmlNode& node)
{
    auto type = node.get<std::string>(MediaResources::Type);
    auto render = node.get<std::string>(MediaResources::Render);

    return {type, render};
}

int MediaParser::idFrom(const XmlNode& node)
{
    return node.get<int>(MediaResources::Id);
}

Uri MediaParser::uriFrom(const XmlNode& node)
{
    auto uri = node.get_optional<std::string>(MediaResources::Uri);
    if (uri)
    {
        Resource fullPath{uri.value()};

        if (!FileSystem::isRegularFile(fullPath)) return Uri::fromString(uri.value());

        return Uri::fromFile(fullPath);
    }
    throw Error{"MediaParser", "Uri is empty"};
}

int MediaParser::durationFrom(const XmlNode& node)
{
    return node.get<int>(MediaResources::Duration);
}

MediaGeometry MediaParser::geometryFrom(const XmlNode& /*node*/)
{
    return MediaGeometry{MediaGeometry::ScaleType::Scaled, MediaGeometry::Align::Left, MediaGeometry::Valign::Top};
}

bool MediaParser::statFrom(const XmlNode& node)
{
    return node.get<bool>(MediaResources::EnableStat, DefaultEnableStat);
}

void MediaParser::attach(Xibo::Media& media, const XmlNode& node)
{
    for (auto [nodeName, attachedNode] : node)
    {
        MediaOptions::Type type{nodeName + "node", MediaResources::NativeRender};
        auto parser = MediaParsersRepo::get(type);

        if (parser)
        {
            media.attach(parser->mediaFrom(attachedNode, 0, 0, globalStatEnabled_));  // TODO: remove 0, 0
        }
    }
}

template <Transition::Heading heading>
std::unique_ptr<TransitionExecutor> MediaParser::createTransition(Transition::Type type,
                                                                  Transition::Direction direction,
                                                                  int duration,
                                                                  const std::shared_ptr<Xibo::Widget>& view)
{
    switch (type)
    {
        case Transition::Type::Fly: return std::make_unique<FlyTransitionExecutor>(heading, direction, duration, view);
        case Transition::Type::Fade: return std::make_unique<FadeTransitionExecutor>(heading, duration, view);
    }

    return nullptr;
}

std::unique_ptr<TransitionExecutor> MediaParser::inTransitionFrom(const XmlNode& node,
                                                                  const std::shared_ptr<Xibo::Widget>& view)
{
    if (auto type = node.get_optional<Transition::Type>(MediaResources::Tranisiton::InType))
    {
        auto direction =
            node.get<Transition::Direction>(MediaResources::Tranisiton::InDirection, Transition::Direction::N);
        int duration = node.get<int>(MediaResources::Tranisiton::InDuration);

        return createTransition<Transition::Heading::In>(type.value(), direction, duration, view);
    }

    return nullptr;
}

std::unique_ptr<TransitionExecutor> MediaParser::outTransitionFrom(const XmlNode& node,
                                                                   const std::shared_ptr<Xibo::Widget>& view)
{
    if (auto type = node.get_optional<Transition::Type>(MediaResources::Tranisiton::OutType))
    {
        auto direction =
            node.get<Transition::Direction>(MediaResources::Tranisiton::OutDirection, Transition::Direction::N);
        int duration = node.get<int>(MediaResources::Tranisiton::OutDuration);

        return createTransition<Transition::Heading::Out>(type.value(), direction, duration, view);
    }

    return nullptr;
}
