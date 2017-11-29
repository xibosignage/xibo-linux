#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"
#include "utilities.hpp"

class Image : public Media
{
public:
    void InitOptions(std::map<std::string, std::string>) override;

    struct Options : Media::BaseOptions
    {
        ScaleType scaleType;
        Align align;
        Valign valign;
    } options;
};


#endif // IMAGE_HPP
