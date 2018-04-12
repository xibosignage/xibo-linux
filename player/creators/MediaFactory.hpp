#pragma once

#include "constants.hpp"
#include <memory>

class Media;
class Region;
struct AnyMedia;
template<MediaType type> struct ParsedMedia;

class MediaFactory
{
public:
    MediaFactory() = delete;
    static std::unique_ptr<Media> create(const Region& region, const AnyMedia& object);

private:
    template<MediaType type>
    static std::unique_ptr<Media> create_media(const Region& region, const ParsedMedia<type>& object);

};
