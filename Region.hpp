#ifndef REGION_HPP
#define REGION_HPP

#include "XiboObject.hpp"
#include "Transition.hpp"
#include "Media.hpp"

#include <map>
#include <vector>
#include <memory>
#include <wx/string.h>

struct Region : XiboObject
{
    void InitOptions(std::map<wxString, wxString> options) override;

    int id;
    int width;
    int height;
    int top;
    int left;
    int zindex;

    struct Options
    {
        bool loop;
        Transition transition;
    } options;

    std::vector<std::shared_ptr<Media>> medias;
};

#endif // REGION_HPP
