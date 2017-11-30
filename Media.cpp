#include "Media.hpp"
#include "utilities.hpp"

Media::Media(int id, int duration, Render render, const std::string& uri) :
    id(id), duration(duration), render(render), uri(uri)
{

}
