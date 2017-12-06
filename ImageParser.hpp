#ifndef IMAGEPARSER_HPP
#define IMAGEPARSER_HPP

#include "MediaParser.hpp"

class ImageParser : public MediaParser<Image>
{
public:
    using MediaParser<Image>::MediaParser;

    std::shared_ptr<Image> create_from_attrs(const boost::property_tree::ptree& options) override;
};

#endif // IMAGEPARSER_HPP
