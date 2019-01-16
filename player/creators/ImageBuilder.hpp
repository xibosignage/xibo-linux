#include "MediaBuilder.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "media/ImageProperties.hpp"
#include "media/Image.hpp"

const std::string DEFAULT_SCALE_TYPE = "center";
const std::string DEFAULT_ALIGN = "center";
const std::string DEFAULT_VALIGN = "middle";

class ImageBuilder : public BaseMediaBuilder<ImageBuilder>
{
public:
    std::unique_ptr<Image> build();

    ImageBuilder& width(int width);
    ImageBuilder& height(int height);
    ImageBuilder& scaleType(const boost::optional<std::string>& scaleType);
    ImageBuilder& align(const boost::optional<std::string>& align);
    ImageBuilder& valign(const boost::optional<std::string>& valign);

protected:
    virtual std::unique_ptr<IImageAdaptor> createAdaptor();

private:
    ImageProperties::ScaleType toScaleType(const std::string& scaleType);
    ImageProperties::Align toAlign(const std::string& align);
    ImageProperties::Valign toValign(const std::string& valign);

private:
    int m_width;
    int m_height;
    ImageProperties::ScaleType m_scaleType;
    ImageProperties::Align m_align;
    ImageProperties::Valign m_valign;

};
