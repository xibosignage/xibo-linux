#include "constants.hpp"
#include "parsers/MediaParser.hpp"
#include <memory>
#include <boost/optional.hpp>

class Media;

class WebViewParser : public MediaParser
{
public:
    WebViewParser(const xlf_node& attrs, const xlf_node& options);
    std::unique_ptr<Media> parse() override;

private:
    boost::optional<int> parse_duration(const std::string& path);
    std::string get_path(int id, const boost::optional<std::string>& uri);

private:
    const xlf_node& m_attrs;
    const xlf_node& m_options;

};
