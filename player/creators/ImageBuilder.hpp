#include "MediaBuilder.hpp"

#include "adaptors/IImageAdaptor.hpp"
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
    MediaGeometry::ScaleType toScaleType(std::string_view scaleType);
    MediaGeometry::Align toAlign(std::string_view align);
    MediaGeometry::Valign toValign(std::string_view valign);

private:
    int m_width;
    int m_height;
    MediaGeometry::ScaleType m_scaleType;
    MediaGeometry::Align m_align;
    MediaGeometry::Valign m_valign;

};
