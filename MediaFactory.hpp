#ifndef MEDIAFACTORY_HPP
#define MEDIAFACTORY_HPP

#include <wx/string.h>

class Media;

class MediaFactory
{
public:
    Media* createMedia(const std::string& type);

};

#endif // MEDIAFACTORY_HPP
