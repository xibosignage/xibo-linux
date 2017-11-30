#include "Layout.hpp"

Layout::Layout(int schemaVersion,
               int width,
               int height,
               const std::string& backgroundImage,
               const std::string& backgroundColor) :
    schemaVersion(schemaVersion),
    width(width),
    height(height),
    backgroundImage(backgroundImage),
    backgroundColor(backgroundColor)
{

}
