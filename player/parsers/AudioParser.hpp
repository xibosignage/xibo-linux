#include "constants.hpp"
#include "parsers/MediaParser.hpp"
#include <memory>

class Media;

class AudioParser : public MediaParser
{
public:
    AudioParser(const xlf_node& parentNode, const xlf_node& mediaNode);

protected:
    std::unique_ptr<Media> doParse() override;

};
