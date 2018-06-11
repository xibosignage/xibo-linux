#include "constants.hpp"
#include "media/Image.hpp"
#include "parsers/MediaParser.hpp"
#include <memory>

class Media;

class ImageParser : public MediaParser
{
public:
    ImageParser(const xlf_node& parent_node, const xlf_node& media_node);

protected:
    std::unique_ptr<Media> doParse() override;

private:
    Image::ScaleType to_scale_type(const std::string& scale_type);
    Image::Align to_align(const std::string& align);
    Image::Valign to_valign(const std::string& valign);

};
