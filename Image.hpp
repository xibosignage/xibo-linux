#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"
#include "utilities.hpp"

class Image : public Media
{
public:
    void InitOptions(std::map<wxString, wxString>) override;

    struct Options : Media::BaseOptions
    {
        ScaleType scaleType;
        Align align;
        Valign valign;
    } options;
};


#endif // IMAGE_HPP
