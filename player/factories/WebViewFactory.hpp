#include "constants.hpp"
#include "factories/MediaFactory.hpp"

#include <memory>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>

class IMedia;

class WebViewFactory : public MediaFactory
{
public:
    WebViewFactory(const xlf_node& parentNode, const xlf_node& mediaNode);

protected:
    std::unique_ptr<IMedia> doCreate() override;

private:
    boost::optional<int> parseDuration(const boost::filesystem::path& path);

};
