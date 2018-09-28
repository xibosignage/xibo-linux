#include "constants.hpp"
#include "creators/MediaFactory.hpp"
#include <memory>

class IMedia;

class AudioFactory : public MediaFactory
{
public:
    AudioFactory(const xlf_node& mediaNode);

protected:
    std::unique_ptr<IMedia> doCreate() override;

};
