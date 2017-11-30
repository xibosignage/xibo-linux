#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "constants.hpp"

#include <map>

class Media
{
public:
    Media(int id, int duration, Render render, const std::string& uri);
    virtual ~Media() = default;

    int id;
    int duration;
    Render render;
    std::string uri;

};

#endif // MEDIA_HPP
