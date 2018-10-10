#include "constants.hpp"
#include "creators/MediaFactory.hpp"
#include <memory>

class IMedia;

class VideoFactory : public MediaFactory
{
public:
    VideoFactory(const xlf_node& parentNode, const xlf_node& mediaNode);

protected:
    std::unique_ptr<IMedia> doCreate() override;

};
