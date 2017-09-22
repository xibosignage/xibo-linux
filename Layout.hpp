#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include "XiboObject.hpp"
#include "Region.hpp"

#include <wx/string.h>
#include <vector>
#include <memory>

class Layout : public XiboObject
{
public:
    void InitOptions(std::map<wxString, wxString>) override { }

    int schemaVersion;
    int width;
    int height;
    wxString backgroundImage;
    wxString backgroundColor;

    std::vector<std::shared_ptr<Region>> regions;
};

#endif // LAYOUT_HPP
