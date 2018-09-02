#include "constants.hpp"
#include "parsers/MediaParser.hpp"

#include <memory>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>

class Media;

class WebViewParser : public MediaParser
{
public:
    WebViewParser(const xlf_node& parentNode, const xlf_node& mediaNode);

protected:
    std::unique_ptr<Media> doParse() override;

private:
    boost::optional<int> parseDuration(const boost::filesystem::path& path);

};
