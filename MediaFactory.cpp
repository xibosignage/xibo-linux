#include "MediaFactory.hpp"

#include "Media.hpp"
#include "Video.hpp"
#include "Image.hpp"

Media* MediaFactory::createMedia(const wxString& type)
{
    if(type == "image")
    {
        return new Image;
    }
    else if(type == "video")
    {
        return new Video;
    }
    else if(type == "audio")
    {
        return new Media;
    }
    return new Media;
}
