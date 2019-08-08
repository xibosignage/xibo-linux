#pragma once

#include "../Field.hpp"

class FilePath;

class CmsSettings
{
public:
    void loadFrom(const FilePath& file);
    void saveTo(const FilePath& file);

    Field<std::string> cmsAddress{"cmsAddress", "http://localhost"};
    Field<std::string> key{"key", "yourserverkey"};
    Field<std::string> resourcesPath{"localLibrary"};
    Field<std::string> username{"username"};
    Field<std::string> password{"password"};
    Field<std::string> domain{"domain"};
    Field<std::string> displayId{"displayId"};
};
