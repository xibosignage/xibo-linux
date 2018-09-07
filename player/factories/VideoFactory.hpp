#include "constants.hpp"
#include "factories/MediaFactory.hpp"
#include <memory>

class IMedia;

class VideoFactory : public MediaFactory
{
public:
    VideoFactory(const xlf_node& mediaNode);

protected:
    std::unique_ptr<IMedia> doCreate() override;

};
