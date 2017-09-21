#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "XiboObject.hpp"

#include <map>
#include <wx/string.h>

class Media : public XiboObject
{
public:
    virtual void InitOptions(std::map<wxString, wxString> options);

    enum Render
    {
        HTML,
        Native
    } render;

    int id;
    int duration;

    struct BaseOptions
    {
        wxString uri;
    } options;

};

#endif // MEDIA_HPP
