#ifndef VIDEOPARSER_HPP
#define VIDEOPARSER_HPP

#include "MediaParser.hpp"
#include "Video.hpp"

class VideoParser : public MediaParser<Video>
{
public:
    using MediaParser<Video>::MediaParser;

    std::shared_ptr<Video> create_from_attrs(const boost::property_tree::ptree& attrs) override;
};

#endif // VIDEOPARSER_HPP
