#include "MediaBuilder.hpp"
#include "media/Image.hpp"

class ImageBuilder : public MediaBuilder
{
public:
    ImageBuilder& width(int width);
    ImageBuilder& height(int height);
    ImageBuilder& scaleType(const boost::optional<std::string>& scaleType);
    ImageBuilder& align(const boost::optional<std::string>& align);
    ImageBuilder& valign(const boost::optional<std::string>& valign);

protected:
    std::unique_ptr<IMedia> doBuild() override;

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
