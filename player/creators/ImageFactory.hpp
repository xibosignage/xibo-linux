#include "constants.hpp"
#include "media/Image.hpp"
#include "creators/MediaFactory.hpp"
#include <memory>

class IMedia;

class ImageFactory : public MediaFactory
{
public:
    ImageFactory(const xlf_node& parentNode, const xlf_node& mediaNode);

protected:
    std::unique_ptr<IMedia> doCreate() override;

private:
    ImageProperties::ScaleType toScaleType(const std::string& scaleType);
    ImageProperties::Align toAlign(const std::string& align);
    ImageProperties::Valign toValign(const std::string& valign);

};
