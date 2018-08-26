#include "constants.hpp"
#include "parsers/MediaParser.hpp"

#include <memory>
#include <boost/optional/optional.hpp>
#include <boost/filesystem/path.hpp>

class Media;

class WebViewParser : public MediaParser
{
public:
    WebViewParser(const xlf_node& parent_node, const xlf_node& media_node);

protected:
    std::unique_ptr<Media> doParse() override;

private:
    boost::optional<int> parse_duration(const boost::filesystem::path& path);

};
