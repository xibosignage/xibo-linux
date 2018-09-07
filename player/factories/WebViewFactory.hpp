#include "constants.hpp"
#include "factories/MediaFactory.hpp"

#include <memory>
#include <boost/optional/optional.hpp>
#include <filesystem>

class IMedia;

class WebViewFactory : public MediaFactory
{
public:
    WebViewFactory(const xlf_node& mediaNode);

protected:
    std::unique_ptr<IMedia> doCreate() override;

private:
    boost::optional<int> parseDuration(const std::filesystem::path& path);

};
