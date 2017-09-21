#include "MediaFactory.hpp"

#include "Media.hpp"
#include "Video.hpp"

Media* MediaFactory::createMedia(const wxString& type)
{
    if(type == "image")
    {
        return new Media;
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
