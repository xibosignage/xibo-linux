#pragma once

#include "MediaRepo.hpp"

class MediaParser;

class MediaParsersRepo : public MediaRepo<MediaParsersRepo, MediaParser>
{
public:
    static void init();

};
