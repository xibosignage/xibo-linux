#include "constants.hpp"
#include "media/Image.hpp"
#include "parsers/MediaParser.hpp"
#include <memory>

class Media;

class ImageParser : public MediaParser
{
public:
    ImageParser(const xlf_node& attrs, const xlf_node& options);
    std::unique_ptr<Media> parse() override;

private:
    Image::ScaleType to_scale_type(const std::string& scale_type);
    Image::Align to_align(const std::string& align);
    Image::Valign to_valign(const std::string& valign);

private:
    const xlf_node& m_attrs;
    const xlf_node& m_options;

};
