#include "MediaBuilder.hpp"

#include "adaptors/GtkImageAdaptor.hpp"
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
    using DefaultMediaHandler = GtkImageAdaptor;
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

private:    
    MediaGeometry::ScaleType getScaleTypeOption(const boost::optional<std::string>& scaleTypeOpt);
    MediaGeometry::Align getAlignOption(const boost::optional<std::string>& alignOpt);
    MediaGeometry::Valign getValignOption(const boost::optional<std::string>& valignOpt);

private:
    int m_width;
    int m_height;
    MediaGeometry::ScaleType m_scaleType;
    MediaGeometry::Align m_align;
    MediaGeometry::Valign m_valign;

};
