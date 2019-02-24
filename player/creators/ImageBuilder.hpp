#include "MediaBuilder.hpp"

#include "adaptors/IImageAdaptor.hpp"
#include "media/Image.hpp"
#include "options/ImageOptions.hpp"

const std::string DEFAULT_SCALE_TYPE = "center";
const std::string DEFAULT_ALIGN = "center";
const std::string DEFAULT_VALIGN = "middle";

class ImageBuilder;

template<>
struct BuilderTraits<ImageBuilder>
{
    using Component = Image;
    using Handler = IImageAdaptor;
    using Options = ImageOptions;
};

class ImageBuilder : public AbstractMediaBuilder<ImageBuilder>
{
public:
    ImageBuilder& width(int width);
    ImageBuilder& height(int height);

protected:
    ImageBuilder& retrieveMediaOptions(const ImageOptions& opts) override;
    std::unique_ptr<IImageAdaptor> createDefaultHandler() override;
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
