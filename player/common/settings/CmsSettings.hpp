#pragma once

#include "common/Field.hpp"

struct CmsSettings
{
    Field<std::string> cmsAddress{"cmsAddress", "http://localhost"};
    Field<std::string> key{"key", "yourserverkey"};
    Field<std::string> resourcesPath{"localLibrary"};
    Field<std::string> username{"username"};
    Field<std::string> password{"password"};
    Field<std::string> domain{"domain"};
    Field<std::string> displayId{"displayId"};
};
