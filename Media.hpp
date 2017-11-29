#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "XiboObject.hpp"
#include "constants.hpp"

#include <map>

class Media : public XiboObject
{
public:
    Media() = default;

    Render render;

    int id;
    int duration;
    std::string uri;

};

#endif // MEDIA_HPP
