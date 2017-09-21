#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Media.hpp"
#include "utilities.hpp"

class Image : public Media
{
public:
    void InitOptions(std::map<wxString, wxString> options) override;

    struct Options : Media::BaseOptions
    {
        int scaleType;
        int align;
        int valign;
    } options;
};


#endif // IMAGE_HPP
