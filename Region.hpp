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
    int id;
    int width;
    int height;
    int top;
    int left;
    int zindex;

    bool loop;
    Transition transition;

    std::vector<std::shared_ptr<Media>> medias;
};

#endif // REGION_HPP
