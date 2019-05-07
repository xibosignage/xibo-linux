#pragma once

#include "Media.hpp"

class MediaFactory
{
public:
    virtual ~MediaFactory() = default;
    virtual std::unique_ptr<Media> create() = 0;
};
