#include "ImageFactory.hpp"

#include "media/IMedia.hpp"
#include "adaptors/GtkImageAdaptor.hpp"

#include "utils/Resources.hpp"
#include "utils/Helpers.hpp"

ImageFactory::ImageFactory(const xlf_node& parentNode, const xlf_node& mediaNode) :
    MediaFactory(parentNode, mediaNode)
{
}

std::unique_ptr<IMedia> ImageFactory::doCreate()
{
    auto path = Resources::directory() / options().get<std::string>("uri");
    int duration = attrs().get<int>("duration");
    int width = static_cast<int>(parentNode().get_child("<xmlattr>").get<double>("width"));
    int height = static_cast<int>(parentNode().get_child("<xmlattr>").get<double>("height"));

    auto scaleType = toScaleType(options().get<std::string>("scaleType", "center"));
    auto align = toAlign(options().get<std::string>("align", "center"));
    auto valign = toValign(options().get<std::string>("valign", "middle"));

    auto adaptor = std::make_unique<GtkImageAdaptor>();
    adaptor->setSize(width, height);
    adaptor->setImage(path.string());

    auto image = std::make_unique<Image>(scaleType, align, valign, std::move(adaptor));
    image->setDuration(duration);
    return image;
}

Image::ScaleType ImageFactory::toScaleType(const std::string& scale_type)
{
    if(scale_type == "center")
        return Image::ScaleType::Scaled;
    else if(scale_type == "stretch")
        return Image::ScaleType::Stretch;
    else
        return Image::ScaleType::Invalid;
}

Image::Align ImageFactory::toAlign(const std::string& align)
{
    if(align == "left")
        return Image::Align::Left;
    else if(align == "center")
        return Image::Align::Center;
    else if(align == "right")
        return Image::Align::Right;
    else
        return Image::Align::Invalid;
}

Image::Valign ImageFactory::toValign(const std::string& valign)
{
    if(valign == "top")
        return Image::Valign::Top;
    else if(valign == "middle")
        return Image::Valign::Middle;
    else if(valign == "bottom")
        return Image::Valign::Bottom;
    else
        return Image::Valign::Invalid;
}
