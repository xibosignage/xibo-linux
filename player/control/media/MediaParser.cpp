#include "MediaParser.hpp"
#include "MediaCreatorsRepo.hpp"
#include "MediaFactory.hpp"
#include "MediaResources.hpp"

#include "control/common/Validators.hpp"
#include "control/common/transitions/FadeTransitionExecutor.hpp"
#include "control/common/transitions/FlyTransitionExecutor.hpp"

std::istream& operator >>(std::istream& in, MediaGeometry::ScaleType& scaleType)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Geometry::Scaled || temp == ResourcesXlf::Media::Geometry::Aspect)
        scaleType = MediaGeometry::ScaleType::Scaled;
    else if(temp == ResourcesXlf::Media::Geometry::Stretch)
        scaleType = MediaGeometry::ScaleType::Stretch;

    return in;
}

std::istream& operator >>(std::istream& in, MediaGeometry::Align& align)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Geometry::LeftAlign)
        align = MediaGeometry::Align::Left;
    else if(temp == ResourcesXlf::Media::Geometry::CenterAlign)
        align = MediaGeometry::Align::Center;
    else if(temp == ResourcesXlf::Media::Geometry::RightAlign)
        align = MediaGeometry::Align::Right;

    return in;
}

std::istream& operator >>(std::istream& in, MediaGeometry::Valign& valign)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Geometry::TopValign)
        valign = MediaGeometry::Valign::Top;
    else if(temp == ResourcesXlf::Media::Geometry::MiddleValign)
        valign = MediaGeometry::Valign::Middle;
    else if(temp == ResourcesXlf::Media::Geometry::BottomValign)
        valign = MediaGeometry::Valign::Bottom;

    return in;
}

std::istream& operator >>(std::istream& in, Transition::Type& type)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Tranisiton::Fly)
        type = Transition::Type::Fly;
    else if(temp == ResourcesXlf::Media::Tranisiton::FadeIn || temp == ResourcesXlf::Media::Tranisiton::FadeOut)
        type = Transition::Type::Fade;

    return in;
}

std::istream& operator >>(std::istream& in, Transition::Direction& direction)
{
    std::string temp;
    in >> temp;

    if(temp == ResourcesXlf::Media::Tranisiton::N)
        direction = Transition::Direction::N;
    else if(temp == ResourcesXlf::Media::Tranisiton::NE)
        direction = Transition::Direction::NE;
    else if(temp == ResourcesXlf::Media::Tranisiton::E)
        direction = Transition::Direction::E;
    else if(temp == ResourcesXlf::Media::Tranisiton::SE)
        direction = Transition::Direction::SE;
    else if(temp == ResourcesXlf::Media::Tranisiton::S)
        direction = Transition::Direction::S;
    else if(temp == ResourcesXlf::Media::Tranisiton::SW)
        direction = Transition::Direction::SW;
    else if(temp == ResourcesXlf::Media::Tranisiton::W)
        direction = Transition::Direction::W;
    else if(temp == ResourcesXlf::Media::Tranisiton::NW)
        direction = Transition::Direction::NW;

    return in;
}

std::unique_ptr<IMedia> MediaParser::mediaFrom(const xml_node& node)
{
    auto baseOptions = baseOptionsFrom(node);
    auto media = createMedia(baseOptions, parseExtraOptions(node));

    media->setInTransition(parseInTransition(node, media->view()));
    media->setOutTransition(parseOutTransition(node, media->view()));

    attachAdditionalMedia(*media, node);

    return media;
}

ExtraOptions MediaParser::parseExtraOptions(const xml_node& node)
{
    auto options = parseExtraOptionsImpl(node);

    options.emplace(ResourcesXlf::Media::Width, node.get<std::string>(ResourcesXlf::Media::Width));
    options.emplace(ResourcesXlf::Media::Height, node.get<std::string>(ResourcesXlf::Media::Height));

    return options;
}

std::unique_ptr<IMedia> MediaParser::createMedia(const MediaOptions& options, const ExtraOptions& extraOptions)
{
    auto factory = MediaCreatorsRepo::get<MediaFactory>(options.type);

    if(factory)
    {
        return factory->create(options, extraOptions);
    }

    return nullptr;
}

MediaOptions MediaParser::baseOptionsFrom(const xml_node& node)
{
    MediaOptions options;

    options.type = typeFrom(node);
    options.id = idFrom(node);
    options.uri = uriFrom(node);
    options.duration = durationFrom(node);
    options.geometry = geometryFrom(node);

    return options;
}

MediaOptions::Type MediaParser::typeFrom(const xml_node& node)
{
    auto type = node.get<std::string>(ResourcesXlf::Media::Type);
    auto render = node.get<std::string>(ResourcesXlf::Media::Render);

    return {type, render};
}

int MediaParser::idFrom(const xml_node& node)
{
    return node.get<int>(ResourcesXlf::Media::Id);
}

Uri MediaParser::uriFrom(const xml_node& node)
{
    auto uri = node.get_optional<std::string>(ResourcesXlf::Media::Uri);
    return Validators::validateUri(uri);
}

int MediaParser::durationFrom(const xml_node& node)
{
    return node.get<int>(ResourcesXlf::Media::Duration);
}

MediaGeometry MediaParser::geometryFrom(const xml_node& /*node*/)
{
    return MediaGeometry{MediaGeometry::ScaleType::Scaled, MediaGeometry::Align::Left, MediaGeometry::Valign::Top};
}

void MediaParser::attachAdditionalMedia(IMedia& media, const xml_node& node)
{
    for(auto [nodeName, attachedNode] : node)
    {
        MediaOptions::Type type{nodeName, ResourcesXlf::Media::NativeRender};
        auto&& parser = MediaCreatorsRepo::get<MediaParser>(type);

        if(parser)
        {
            media.attach(parser->mediaFrom(attachedNode));
        }
    }
}

template<Transition::Heading heading>
std::unique_ptr<TransitionExecutor> MediaParser::createTransition(Transition::Type type,
                                                                  Transition::Direction direction,
                                                                  int duration,
                                                                  const std::shared_ptr<IWidget>& view)
{
    switch(type)
    {
        case Transition::Type::Fly:
            return std::make_unique<FlyTransitionExecutor>(heading, direction, duration, view);
        case Transition::Type::Fade:
            return std::make_unique<FadeTransitionExecutor>(heading, duration, view);
    }

    return nullptr;
}

std::unique_ptr<TransitionExecutor> MediaParser::parseInTransition(const xml_node& node, const std::shared_ptr<IWidget>& view)
{
    if(auto type = node.get_optional<Transition::Type>(ResourcesXlf::Media::Tranisiton::InType))
    {
        auto direction = node.get<Transition::Direction>(ResourcesXlf::Media::Tranisiton::InDirection);
        int duration = node.get<int>(ResourcesXlf::Media::Tranisiton::InDuration);

        return createTransition<Transition::Heading::In>(type.value(), direction, duration, view);
    }

    return nullptr;
}

std::unique_ptr<TransitionExecutor> MediaParser::parseOutTransition(const xml_node& node, const std::shared_ptr<IWidget>& view)
{
    if(auto type = node.get_optional<Transition::Type>(ResourcesXlf::Media::Tranisiton::OutType))
    {
        auto direction = node.get<Transition::Direction>(ResourcesXlf::Media::Tranisiton::OutDirection);
        int duration = node.get<int>(ResourcesXlf::Media::Tranisiton::OutDuration);

        return createTransition<Transition::Heading::Out>(type.value(), direction, duration, view);
    }

    return nullptr;
}
