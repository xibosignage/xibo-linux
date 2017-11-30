#ifndef REGION_HPP
#define REGION_HPP

#include "Transition.hpp"
#include "Media.hpp"

#include <map>
#include <vector>
#include <memory>
#include <wx/string.h>

class Region
{
public:
    Region(int id, int width, int height, int top, int left, int zindex, bool loop);

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
