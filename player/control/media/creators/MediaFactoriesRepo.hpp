#pragma once

#include "MediaRepo.hpp"

class MediaFactory;

class MediaFactoriesRepo : public MediaRepo<MediaFactoriesRepo, MediaFactory>
{
public:
    static void init();
};
