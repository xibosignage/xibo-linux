#ifndef XIBOOBJECT_HPP
#define XIBOOBJECT_HPP

#include <map>
#include <wx/string.h>

class XiboObject
{
public:
    XiboObject() = default;
    virtual void InitOptions(std::map<wxString, wxString> options) = 0;

};

#endif // XIBOOBJECT_HPP
