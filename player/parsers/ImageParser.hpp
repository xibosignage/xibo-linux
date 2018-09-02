#include "constants.hpp"
#include "media/Image.hpp"
#include "parsers/MediaParser.hpp"
#include <memory>

class Media;

class ImageParser : public MediaParser
{
public:
    ImageParser(const xlf_node& parentNode, const xlf_node& mediaNode);

protected:
    std::unique_ptr<Media> doParse() override;

private:
    Image::ScaleType toScaleType(const std::string& scaleType);
    Image::Align toAlign(const std::string& align);
    Image::Valign toValign(const std::string& valign);

};
