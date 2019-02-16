#include "MediaBuilder.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "media/ImageProperties.hpp"
#include "media/Image.hpp"
#include "parsers/ImageOptions.hpp"

const std::string DEFAULT_SCALE_TYPE = "center";
const std::string DEFAULT_ALIGN = "center";
const std::string DEFAULT_VALIGN = "middle";

class ImageBuilder;

template<>
struct BuilderTraits<ImageBuilder>
{
    using Media = Image;
    using MediaHandler = IImageAdaptor;
    using Options = ResourcesXlf::ImageOptions;
};

class ImageBuilder : public BaseMediaBuilder<ImageBuilder>
{
public:
    ImageBuilder& width(int width);
    ImageBuilder& height(int height);

protected:
    ImageBuilder& mediaOptions(const ResourcesXlf::ImageOptions& opts) override;
    std::unique_ptr<Image> create() override;
    std::unique_ptr<IImageAdaptor> createHandler() override;

private:    
    ImageProperties::ScaleType getScaleTypeOption(const boost::optional<std::string>& scaleTypeOpt);
    ImageProperties::Align getAlignOption(const boost::optional<std::string>& alignOpt);
    ImageProperties::Valign getValignOption(const boost::optional<std::string>& valignOpt);

private:
    int m_width;
    int m_height;
    ImageProperties::ScaleType m_scaleType;
    ImageProperties::Align m_align;
    ImageProperties::Valign m_valign;

};
