#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "XiboObject.hpp"
#include "constants.hpp"

#include <map>
#include <wx/string.h>

class Media : public XiboObject
{
public:
    void InitOptions(std::map<wxString, wxString> options) override;

    Render render;

    int id;
    int duration;

    struct BaseOptions
    {
        wxString uri;
    } options;

};

#endif // MEDIA_HPP
