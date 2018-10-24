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
    ImageProperties props{scaleType, align, valign};

    auto image = std::make_unique<Image>(width, height, path, props, std::make_unique<GtkImageAdaptor>());
    image->setDuration(duration);
    return image;
}

ImageProperties::ScaleType ImageFactory::toScaleType(const std::string& scale_type)
{
    if(scale_type == "center")
        return ImageProperties::ScaleType::Scaled;
    else if(scale_type == "stretch")
        return ImageProperties::ScaleType::Stretch;
    else
        return ImageProperties::ScaleType::Invalid;
}

ImageProperties::Align ImageFactory::toAlign(const std::string& align)
{
    if(align == "left")
        return ImageProperties::Align::Left;
    else if(align == "center")
        return ImageProperties::Align::Center;
    else if(align == "right")
        return ImageProperties::Align::Right;
    else
        return ImageProperties::Align::Invalid;
}

ImageProperties::Valign ImageFactory::toValign(const std::string& valign)
{
    if(valign == "top")
        return ImageProperties::Valign::Top;
    else if(valign == "middle")
        return ImageProperties::Valign::Middle;
    else if(valign == "bottom")
        return ImageProperties::Valign::Bottom;
    else
        return ImageProperties::Valign::Invalid;
}
